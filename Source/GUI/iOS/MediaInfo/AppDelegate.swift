/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

import UIKit
import StoreKit
import CoreData

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate, UISplitViewControllerDelegate, SKPaymentTransactionObserver {
    var window: UIWindow?
    let subscriptionManager = SubscriptionManager.shared

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
        let splitViewController = self.window!.rootViewController as! UISplitViewController
        let navigationController = splitViewController.viewControllers[splitViewController.viewControllers.count-1] as! UINavigationController
        navigationController.topViewController!.navigationItem.leftBarButtonItem = splitViewController.displayModeButtonItem
        splitViewController.delegate = self

        let reportsListNavigationController = splitViewController.viewControllers[0] as! UINavigationController
        let controller = reportsListNavigationController.topViewController as! ReportsListViewController
        controller.managedObjectContext = self.persistentContainer.viewContext

        SKPaymentQueue.default().add(self)
        subscriptionManager.loadSubscription()

        return true
    }

    func applicationWillResignActive(_ application: UIApplication) {
    }

    func applicationDidEnterBackground(_ application: UIApplication) {
    }

    func applicationWillEnterForeground(_ application: UIApplication) {
    }

    func applicationDidBecomeActive(_ application: UIApplication) {
    }

    func applicationWillTerminate(_ application: UIApplication) {
        // Called when the application is about to terminate. Save data if appropriate.
        self.saveContext()
    }

    // MARK: - Split view

    func splitViewController(_ splitViewController: UISplitViewController, collapseSecondary secondaryViewController:UIViewController, onto primaryViewController:UIViewController) -> Bool {
        guard let secondaryAsNavController = secondaryViewController as? UINavigationController else { return false }
        guard let topAsReportController = secondaryAsNavController.topViewController as? ReportViewController else { return false }
        if topAsReportController.report == nil {
            // Return true to indicate that we have handled the collapse by doing nothing; the secondary controller will be discarded.
            return true
        }
        return false
    }

    // MARK: - Core Data stack

    lazy var persistentContainer: NSPersistentContainer = {
        /*
         The persistent container for the application. This implementation
         creates and returns a container, having loaded the store for the
         application to it. This property is optional since there are legitimate
         error conditions that could cause the creation of the store to fail.
        */
        let container = NSPersistentContainer(name: "MediaInfo")
        container.loadPersistentStores(completionHandler: { (storeDescription, error) in
            if let error = error as NSError? {
                /*
                 Typical reasons for an error here include:
                 * The parent directory does not exist, cannot be created, or disallows writing.
                 * The persistent store is not accessible, due to permissions or data protection when the device is locked.
                 * The device is out of space.
                 * The store could not be migrated to the current model version.
                 Check the error message to determine what the actual problem was.
                 */
                NSLog("ERROR: unable to create or access persistent container, %@", error)
            }
        })
        return container
    }()

    // MARK: - Core Data Saving support

    func saveContext () {
        let context = persistentContainer.viewContext
        if context.hasChanges {
            do {
                try context.save()
            } catch {
                NSLog("ERROR: unable to save context, %@", error as NSError)
            }
        }
    }

    // MARK: - SKPaymentTransactionObserver
    func paymentQueue(_ queue: SKPaymentQueue, updatedTransactions transactions: [SKPaymentTransaction]) {
        for transaction in transactions {
            switch transaction.transactionState {
            case .purchasing:
                handlePurchasingState(for: transaction, in: queue)
            case .purchased:
                handlePurchasedState(for: transaction, in: queue)
            case .restored:
                handleRestoredState(for: transaction, in: queue)
            case .failed:
                handleFailedState(for: transaction, in: queue)
            case .deferred:
                handleDeferredState(for: transaction, in: queue)
            }
        }
    }

    func paymentQueue(_ queue: SKPaymentQueue, shouldAddStorePayment payment: SKPayment, for product: SKProduct) -> Bool {
        return true
    }

    func handlePurchasingState(for transaction: SKPaymentTransaction, in queue: SKPaymentQueue) {
    }

    func handlePurchasedState(for transaction: SKPaymentTransaction, in queue: SKPaymentQueue) {
        queue.finishTransaction(transaction)
        SubscriptionManager.shared.purchased()
    }

    func handleRestoredState(for transaction: SKPaymentTransaction, in queue: SKPaymentQueue) {
        queue.finishTransaction(transaction)
        SubscriptionManager.shared.restored()
    }

    func handleFailedState(for transaction: SKPaymentTransaction, in queue: SKPaymentQueue) {
        queue.finishTransaction(transaction)
        SubscriptionManager.shared.purchaseFailed()
    }

    func handleDeferredState(for trasaction: SKPaymentTransaction, in queue: SKPaymentQueue) {
        SubscriptionManager.shared.purchaseDeferred()
    }
}
