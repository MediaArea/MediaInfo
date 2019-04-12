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

    var subscriptionDetails: SKProduct? {
        didSet {
            NotificationCenter.default.post(name: .subscriptionDetailsReady, object: nil)
        }
    }

    var subscriptionActive: Bool = false

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

                    subscriptionActive = newDate >= Date()
                    NotificationCenter.default.post(name: .subscriptionStateChanged, object: nil)
                }
            }
        }
    }

    var shouldNotifyUserForSubscriptionEnd: Bool {
        get {
            if let end = subscriptionEndDate, end < Date() {
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

        parseSubscriptions()

        if let endDate = subscriptionEndDate {
            subscriptionActive = endDate >= Date()
        }
    }

    func notifyUserForSubscriptionEnd(parent: UIViewController & SubscribeResultDelegate) {
        NSUbiquitousKeyValueStore.default.set([Date()], forKey: "SubscriptionEndUserNotificationDate")
        let controller = UIAlertController(title: "Renew subscription?", message: "Your subscription has just ended.", preferredStyle: .alert)
        controller.addAction(UIAlertAction(title: "Renew", style: .default, handler: { _ in
            let storyboard = UIStoryboard(name: "Main", bundle: Bundle.main)
            if let subscribeViewController = storyboard.instantiateViewController(withIdentifier: "SubscribeViewController") as? SubscribeViewController {
                let navigationController = UINavigationController(rootViewController: subscribeViewController)
                subscribeViewController.delegate = parent

                parent.present(navigationController, animated: true, completion: nil)
            }
        }))
        controller.addAction(UIAlertAction(title: "Close", style: .cancel))

        parent.present(controller, animated: true)
    }

    func purchase() {
        if let subscription = subscriptionDetails {
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
        let request = SKProductsRequest(productIdentifiers: Set([subscriptionId]))
        request.delegate = self
        request.start()
    }

    func productsRequest(_ request: SKProductsRequest, didReceive response: SKProductsResponse) {
        for product in response.products {
            if product.productIdentifier == subscriptionId {
                subscriptionDetails = product
            }
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
                parseSubscriptions()
            } catch {
                NSLog("ERROR: unable to decode receipt, %@", error as NSError)
            }
        }
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
