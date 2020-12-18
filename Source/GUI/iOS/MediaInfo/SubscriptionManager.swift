/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

import Foundation
import StoreKit

import TPInAppReceipt

extension Notification.Name {
    static let subscriptionDetailsReady = Notification.Name("net.mediaarea.mediainfo.ios.notifications.subscriptionDetailsReady")
    static let subscriptionDetailsUnaviable = Notification.Name("net.mediaarea.mediainfo.ios.notifications.subscriptionDetailsUnaviable")
    static let purchaseSucceeded = Notification.Name("net.mediaarea.mediainfo.ios.notifications.purchaseSucceeded")
    static let purchaseFailed = Notification.Name("net.mediaarea.mediainfo.ios.notifications.purchaseFailed")
    static let purchaseDeferred = Notification.Name("net.mediaarea.mediainfo.ios.notifications.purchaseDeferred")
    static let restoreFinished = Notification.Name("net.mediaarea.mediainfo.ios.notifications.restoreFinished")
    static let subscriptionStateChanged = Notification.Name("net.mediaarea.mediainfo.ios.notifications.subscriptionStateChanged")
}

class SubscriptionManager : NSObject, SKProductsRequestDelegate {
    static let shared = SubscriptionManager()

    private let subscriptionId = "net.mediaarea.mediainfo.ios.nrsubs.inapp1"
    private let lifetimeSubscriptionId = "net.mediaarea.mediainfo.ios.purchase.inapp1"

    var subscriptionDetails: SKProduct?
    var lifetimeSubscriptionDetails: SKProduct?

    var subscriptionActive: Bool = false {
        didSet(oldValue) {
            if subscriptionActive != oldValue {
                NotificationCenter.default.post(name: .subscriptionStateChanged, object: nil)
            }
        }
    }
    var isLifetime: Bool {
        get {
            if UserDefaults.standard.bool(forKey: "LifetimeSubscription") {
                return true
            }

            if NSUbiquitousKeyValueStore.default.bool(forKey: "LifetimeSubscription") {
                return true
            }

            return false
        }
        set(newValue) {
            if newValue {
                UserDefaults.standard.set(true, forKey: "LifetimeSubscription")
                NSUbiquitousKeyValueStore.default.set(true, forKey: "LifetimeSubscription")
                subscriptionActive = true
            }
        }
    }

    var subscriptions: [Date] {
        get {
            var toReturn: [Date] = []

            if let local = UserDefaults.standard.array(forKey: "Subscriptions") as? [Date] {
                toReturn = local
            }

            if let remote = NSUbiquitousKeyValueStore.default.array(forKey: "Subscriptions") as? [Date] {
                toReturn = Array(Set(toReturn + remote))
            }
            return toReturn
        }
        set(newValue) {
            var toAdd = newValue

            if let local = UserDefaults.standard.array(forKey: "Subscriptions") as? [Date] {
                toAdd = Array(Set(toAdd + local))
            }

            if let remote = NSUbiquitousKeyValueStore.default.array(forKey: "Subscriptions") as? [Date] {
                toAdd = Array(Set(toAdd + remote))
            }

            if toAdd.count > 0 {
                UserDefaults.standard.set(toAdd, forKey: "Subscriptions")
                NSUbiquitousKeyValueStore.default.set(toAdd, forKey: "Subscriptions")
            }
        }
    }

    var codes: [String] {
        get {
            var toReturn: [String] = []

            if let local = UserDefaults.standard.array(forKey: "Codes") as? [String] {
                toReturn = local
            }

            if let remote = NSUbiquitousKeyValueStore.default.array(forKey: "Codes") as? [String] {
                toReturn = Array(Set(toReturn + remote))
            }
            return toReturn
        }
        set(newValue) {
            var toAdd = newValue

            if let local = UserDefaults.standard.array(forKey: "Codes") as? [String] {
                toAdd = Array(Set(toAdd + local))
            }

            if let remote = NSUbiquitousKeyValueStore.default.array(forKey: "Codes") as? [String] {
                toAdd = Array(Set(toAdd + remote))
            }

            if toAdd.count > 0 {
                UserDefaults.standard.set(toAdd, forKey: "Codes")
                NSUbiquitousKeyValueStore.default.set(toAdd, forKey: "Codes")
            }
        }
    }

    var subscriptionEndDate: Date? {
        get {
            if let saved = UserDefaults.standard.array(forKey: "SubscriptionEndDate") as? [Date], saved.count > 0 {
                return saved[0]
            }
            return nil
        }
        set(newValue) {
            if let newDate = newValue {
                if newDate > subscriptionEndDate ?? Date(timeIntervalSince1970: 0.0) {
                    UserDefaults.standard.set([newDate], forKey: "SubscriptionEndDate")
                    subscriptionActive = subscriptionActive || isLifetime || newDate >= Date()

                }
            }
        }
    }

    var shouldNotifyUserForSubscriptionEnd: Bool {
        get {
            if let end = subscriptionEndDate, end < Date() && !isLifetime {
                if let saved = NSUbiquitousKeyValueStore.default.array(forKey: "SubscriptionEndUserNotificationDate") as? [Date], saved.count > 0 {
                    let lastNotificationDate = saved[0]
                    if lastNotificationDate <= end {
                        return true
                    }
                } else {
                    return true
                }
            }
            return false
        }
    }

    override init() {
        super.init()

        NotificationCenter.default.addObserver(self, selector: #selector(didChangeExternally(_:)), name: NSUbiquitousKeyValueStore.didChangeExternallyNotification, object: NSUbiquitousKeyValueStore.default)

        NSUbiquitousKeyValueStore.default.synchronize()

        parseReceipt()

        if let endDate = subscriptionEndDate {
            subscriptionActive = isLifetime || endDate >= Date()
        }
        if isLifetime {
            subscriptionActive = true
        }
    }

    func notifyUserForSubscriptionEnd(parent: UIViewController & SubscribeResultDelegate) {
        NSUbiquitousKeyValueStore.default.set([Date()], forKey: "SubscriptionEndUserNotificationDate")
        let controller = UIAlertController(title: NSLocalizedString("Renew subscription?", tableName: "Core", comment: ""), message: NSLocalizedString("Your subscription has just ended.", tableName: "Core", comment: ""), preferredStyle: .alert)
        controller.addAction(UIAlertAction(title: NSLocalizedString("Renew", tableName: "Core", comment: ""), style: .default, handler: { _ in
            let storyboard = UIStoryboard(name: "Main", bundle: Bundle.main)
            if let subscribeViewController = storyboard.instantiateViewController(withIdentifier: "SubscribeViewController") as? SubscribeViewController {
                let navigationController = UINavigationController(rootViewController: subscribeViewController)
                subscribeViewController.delegate = parent

                parent.present(navigationController, animated: true, completion: nil)
            }
        }))
        controller.addAction(UIAlertAction(title: NSLocalizedString("Close", tableName: "Core", comment: ""), style: .cancel))

        parent.present(controller, animated: true)
    }

    func purchase(product: SKProduct?) {
        if let subscription = product {
            let payment = SKPayment(product: subscription)
            SKPaymentQueue.default().add(payment)
        }
    }

    func purchaseFailed() {
        NotificationCenter.default.post(name: .purchaseFailed, object: nil)
    }

    func purchaseDeferred() {
        NotificationCenter.default.post(name: .purchaseDeferred, object: nil)
    }

    func purchased() {
        parseReceipt()
        NotificationCenter.default.post(name: .purchaseSucceeded, object: nil)
    }

    func restore() {
        SKPaymentQueue.default().restoreCompletedTransactions()
    }

    func restored() {
        parseReceipt()
        NotificationCenter.default.post(name: .restoreFinished, object: nil)
    }

    func loadSubscription() {
        let request = SKProductsRequest(productIdentifiers: Set([subscriptionId, lifetimeSubscriptionId]))
        request.delegate = self
        request.start()
    }

    func productsRequest(_ request: SKProductsRequest, didReceive response: SKProductsResponse) {
        for product in response.products {
            if product.productIdentifier == subscriptionId {
                subscriptionDetails = product
            }
            else if product.productIdentifier == lifetimeSubscriptionId {
                lifetimeSubscriptionDetails = product
            }
        }
        if subscriptionDetails != nil && lifetimeSubscriptionDetails != nil {
            NotificationCenter.default.post(name: .subscriptionDetailsReady, object: nil)
        }
    }

    func request(_ request: SKRequest, didFailWithError error: Error) {
        if type(of: request) == SKProductsRequest.self {
            NotificationCenter.default.post(name: .subscriptionDetailsUnaviable, object: nil)
        }
    }

    func parseSubscriptions() {
        var end = Date(timeIntervalSince1970: 0.0)
        for subscription in subscriptions.sorted(by: { $0.compare($1) == .orderedAscending }) {
            if subscription > end { // new subscription or re-subscribed after the end of previous subscription
                if let new = Calendar.current.date(byAdding: .year, value: 1, to: subscription) {
                    end = new
                }
            } else { // re-subscribed before the end of previous subscription
                if let new = Calendar.current.date(byAdding: .year, value: 1, to: end) {
                    end = new
                }
            }
        }
        if end != Date(timeIntervalSince1970: 0.0) {
            subscriptionEndDate = end
        }
    }

    func parseReceipt() {
        if let receiptData = loadReceipt() {
            do {
                let decodedReceipt = try InAppReceipt.receipt(from: receiptData)
                try decodedReceipt.verify()

                for subscription in decodedReceipt.purchases(ofProductIdentifier: subscriptionId) {
                    if !subscriptions.contains(subscription.purchaseDate) {
                        subscriptions.append(subscription.purchaseDate)
                    }
                }

                if decodedReceipt.purchases(ofProductIdentifier: lifetimeSubscriptionId).count > 0 {
                    isLifetime = true
                }
            } catch {
                NSLog("ERROR: unable to decode receipt, %@", error as NSError)
            }
        }
        parseSubscriptions()
    }

    func loadReceipt() -> Data? {
        guard let receiptUrl = Bundle.main.appStoreReceiptURL else {
            return nil
        }

        do {
            return try Data(contentsOf: receiptUrl)
        } catch {
            return nil
        }
    }

    @objc func didChangeExternally(_ notification: Notification) {
        // trigger syncronisation
        var toAdd: [Date] = []

        if let local = UserDefaults.standard.array(forKey: "Subscriptions") as? [Date] {
            toAdd = Array(Set(toAdd + local))
        }

        if let remote = NSUbiquitousKeyValueStore.default.array(forKey: "Subscriptions") as? [Date] {
            toAdd = Array(Set(toAdd + remote))
        }

        if toAdd.count > 0 {
            UserDefaults.standard.set(toAdd, forKey: "Subscriptions")
            NSUbiquitousKeyValueStore.default.set(toAdd, forKey: "Subscriptions")
        }
    }
}
