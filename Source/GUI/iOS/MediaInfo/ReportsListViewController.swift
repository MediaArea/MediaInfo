/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

import UIKit
import CoreData
import MobileCoreServices

import Toast_Swift
import PopMenu

extension UITableViewController {
    class func displayWelcome(onView: UIView) -> UIView {
        let welcomeView = UILabel.init(frame: onView.bounds)
        welcomeView.text = NSLocalizedString("(You must at least open one file)", tableName: "Core", comment: "")

        if SubscriptionManager.shared.subscriptionActive && Core.shared.darkMode {
            welcomeView.textColor = UIColor.white
        } else {
            welcomeView.textColor = UIColor.black
        }
        welcomeView.textAlignment = .center

        DispatchQueue.main.async {
           onView.addSubview(welcomeView)
        }

        return welcomeView
    }

    class func removeWelcome(view: UIView) {
        DispatchQueue.main.async {
            view.removeFromSuperview()
        }
    }
}
class ReportsListViewController: UITableViewController, NSFetchedResultsControllerDelegate, UIDocumentPickerDelegate, SubscribeResultDelegate {

    var reportViewController: ReportViewController? = nil
    var managedObjectContext: NSManagedObjectContext? = nil
    var selectedReport: Event? = nil
    var welcomeView: UIView? = nil
    var message: String? = nil
    let core: Core = Core()

    override func viewDidLoad() {
        super.viewDidLoad()

        let menuButton = UIBarButtonItem(title: NSLocalizedString("Menu", tableName: "Core", comment: ""), style: .plain, target: self, action: #selector(showMenu(_:)))
        let addButton = UIBarButtonItem(title: NSLocalizedString("Open", tableName: "Core", comment: ""), style: .plain, target: self, action: #selector(insertReport(_:)))

        navigationItem.leftBarButtonItem = menuButton
        navigationItem.rightBarButtonItem = addButton

        if let split = splitViewController {
            split.preferredDisplayMode = .allVisible
            let controllers = split.viewControllers
            reportViewController = (controllers[controllers.count-1] as! UINavigationController).topViewController as? ReportViewController
        }

        // Toggle "tap to dismiss" functionality
        ToastManager.shared.isTapToDismissEnabled = true

        // Toggle queueing behavior
        ToastManager.shared.isQueueEnabled = true

        if SubscriptionManager.shared.subscriptionActive {
            subscriptionActive()
        }

        NotificationCenter.default.addObserver(self, selector: #selector(subscriptionStateChanged(_:)), name: .subscriptionStateChanged, object: nil)
    }

    let fetchRequest: NSFetchRequest<Event> = Event.fetchRequest()

    override func viewWillAppear(_ animated: Bool) {
        clearsSelectionOnViewWillAppear = splitViewController!.isCollapsed
        super.viewWillAppear(animated)

        // Bug, in iOS 11.2 navigation buttons remains faded after segue back
        if #available(iOS 11.2, *) {
            navigationController?.navigationBar.tintAdjustmentMode = .normal
            navigationController?.navigationBar.tintAdjustmentMode = .automatic
        }
    }

    override func viewDidAppear(_ animated: Bool) {
        showHideWelcome()
        if SubscriptionManager.shared.shouldNotifyUserForSubscriptionEnd {
            SubscriptionManager.shared.notifyUserForSubscriptionEnd(parent: self)
        }
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @objc func showMenu(_ sender: Any) {
        let menuViewController = PopMenuViewController(actions: [])
        if SubscriptionManager.shared.subscriptionActive {
            if Core.shared.darkMode {
                menuViewController.addAction(PopMenuDefaultAction(title: "✓ " + NSLocalizedString("Dark mode", tableName: "Core", comment: ""), didSelect: { _ in
                    Core.shared.darkMode = false
                }))
            } else {
                menuViewController.addAction(PopMenuDefaultAction(title: NSLocalizedString("Dark mode", tableName: "Core", comment: ""), didSelect: { _ in
                    Core.shared.darkMode = true
                }))
            }
            menuViewController.addAction(PopMenuDefaultAction(title: NSLocalizedString("Manage subscription", tableName: "Core", comment: ""), didSelect: { [menuViewController] _ in
                menuViewController.didDismiss = { _ in
                    let storyboard = UIStoryboard(name: "Main", bundle: Bundle.main)
                    if let subscribeViewController = storyboard.instantiateViewController(withIdentifier: "SubscribeViewController") as? SubscribeViewController {
                        let navigationController = UINavigationController(rootViewController: subscribeViewController)
                        subscribeViewController.delegate = self

                        self.present(navigationController, animated: true, completion: nil)
                    }
                }
            }))
        //} else {
            if Core.shared.userLocale {
                menuViewController.addAction(PopMenuDefaultAction(title: "✓ " + NSLocalizedString("Translate reports", tableName: "Core", comment: ""), didSelect: { _ in
                    Core.shared.userLocale = false
                }))
            } else {
                menuViewController.addAction(PopMenuDefaultAction(title: NSLocalizedString("Translate reports", tableName: "Core", comment: ""), didSelect: { _ in
                    Core.shared.userLocale = true
                }))
            }
        } else {
            menuViewController.addAction(PopMenuDefaultAction(title: NSLocalizedString("Subscribe", tableName: "Core", comment: ""), didSelect: { [menuViewController] _ in
                menuViewController.didDismiss = { _ in
                    let storyboard = UIStoryboard(name: "Main", bundle: Bundle.main)
                    if let subscribeViewController = storyboard.instantiateViewController(withIdentifier: "SubscribeViewController") as? SubscribeViewController {
                        let navigationController = UINavigationController(rootViewController: subscribeViewController)
                        subscribeViewController.delegate = self

                        self.present(navigationController, animated: true, completion: nil)
                    }
                }
            }))
        }

        if #available(iOS 13.0, *) {
            menuViewController.addAction(PopMenuDefaultAction(title: NSLocalizedString("Language", tableName: "Core", comment: ""), didSelect: { [menuViewController] _ in
                menuViewController.didDismiss = { _ in
                    if let url = URL(string:  UIApplication.openSettingsURLString) {
                        UIApplication.shared.open(url)
                    }
                }
            }))
        }


        menuViewController.addAction(PopMenuDefaultAction(title: NSLocalizedString("About", tableName: "Core", comment: ""), didSelect: { [menuViewController] _ in
            menuViewController.didDismiss = { _ in
                let storyboard = UIStoryboard(name: "Main", bundle: Bundle.main)
                let aboutViewController = storyboard.instantiateViewController(withIdentifier: "AboutViewController")
                let navigationController = UINavigationController(rootViewController: aboutViewController)


                self.present(navigationController, animated: true, completion: nil)
            }
        }))

        present(menuViewController, animated: true, completion: nil)
    }

    func showMessage(message: String?) {
        if message != nil {
            navigationController?.view?.makeToast(message, duration: 5.0, position: .top)
        }
    }

    @objc func insertReport(_ sender: Any) {
        let documentPicker: UIDocumentPickerViewController = UIDocumentPickerViewController(documentTypes: [kUTTypeItem as String], in: .import)
        documentPicker.delegate = self

        //if #available(iOS 11.0, *) {
        //    documentPicker.allowsMultipleSelection = true
        //}

        self.present(documentPicker, animated: true, completion: nil)
    }

    // Because URL.hasDirectoryPath don't work with directories returned by iCloud
    func isDirectoryUrl(url: URL) -> Bool {
        let fileManager: FileManager = FileManager.default
        do {
            let _ = try fileManager.contentsOfDirectory(at: url, includingPropertiesForKeys: nil, options: .init(rawValue: 0))
        } catch {
            return false // Not a directory
        }
        return true
    }

    func retreiveFileFromIcloud(url: URL) -> URL? {
        let fileManager: FileManager = FileManager.default

        var lastPathComponent = url.lastPathComponent
        if (lastPathComponent.hasPrefix(".") && lastPathComponent.hasSuffix(".icloud")) {
                lastPathComponent.removeFirst() // remove .
                lastPathComponent.removeLast(7) // remove .icloud

            let folderUrl: URL = url.deletingLastPathComponent()
            let fileUrl = folderUrl.appendingPathComponent(lastPathComponent)

            // launch the download
            do {
                try fileManager.startDownloadingUbiquitousItem(at: fileUrl)
            } catch {
                print("Unexpected error: \(error).")
                return nil
            }

            var isDownloaded = false
            while !isDownloaded {
                if fileManager.fileExists(atPath: fileUrl.path) {
                    isDownloaded = true
                    return fileUrl
                }
            }
        }

        return nil
    }

    func extractFileUrls(urls: [URL]) -> [URL] {
        let fileManager: FileManager = FileManager.default
        var toReturn: [URL] = [URL]()

        for url in urls {
            if isDirectoryUrl(url: url) {
                let enumerator: FileManager.DirectoryEnumerator? = fileManager.enumerator(at: url, includingPropertiesForKeys: nil)

                while let cur: URL = (enumerator?.nextObject() as? URL) {
                    if !isDirectoryUrl(url: cur) {
                        let lastPathComponent = cur.lastPathComponent
                        if (lastPathComponent.hasPrefix(".") && lastPathComponent.hasSuffix(".icloud")) {
                            if let file: URL = retreiveFileFromIcloud(url: cur) {
                                toReturn.append(file)
                            }
                        } else {
                            toReturn.append(cur)
                        }
                    }
                }
            } else {
                let lastPathComponent = url.lastPathComponent
                if (lastPathComponent.hasPrefix(".") && lastPathComponent.hasSuffix(".icloud")) {
                    if let file: URL = retreiveFileFromIcloud(url: url) {
                        toReturn.append(file)
                    }
                } else {
                    toReturn.append(url)
                }
            }
        }

        return toReturn
    }

    func documentPicker(_ controller: UIDocumentPickerViewController, didPickDocumentsAt urls: [URL]) {
        if urls.count > 0 {
            view.isUserInteractionEnabled = false
            navigationItem.leftBarButtonItem?.isEnabled = false
            navigationItem.rightBarButtonItem?.isEnabled = false

            if let welcome: UIView = welcomeView {
                UITableViewController.removeWelcome(view: welcome)
            }

            view.makeToastActivity(.center)
        }

        DispatchQueue.main.async { [weak self] in
            if let main = self {
                let fileUrls: [URL] = main.extractFileUrls(urls: urls)
                for url in fileUrls {
                     do {
                        let report = try main.core.createReport(url: url, name: url.lastPathComponent)
                        let context = main.fetchedResultsController.managedObjectContext
                        let newEvent = Event(context: context)

                        // Configure the new report.
                        newEvent.timestamp = Date()
                        newEvent.filename = url.lastPathComponent
                        newEvent.report = NSKeyedArchiver.archivedData(withRootObject: report)
                        newEvent.version = main.core.version

                        // Save the context.
                        try context.save()

                        if fileUrls.count == 1 {
                           // main.selectedReport = newEvent
                            main.tableView.selectRow(at: IndexPath(row: main.tableView.numberOfRows(inSection: 0)-1, section: 0), animated: true, scrollPosition: .bottom)
                            main.performSegue(withIdentifier: "showReport", sender: self)
                        }
                    } catch {
                        let nserror = error as NSError

                        main.view.makeToast("ERROR: \(nserror), \(nserror.userInfo) when trying to save report", duration: 5.0, position: .top)
                    }
                }

                main.view.hideToastActivity()
                main.view.isUserInteractionEnabled = true
                main.navigationItem.leftBarButtonItem?.isEnabled = true
                main.navigationItem.rightBarButtonItem?.isEnabled = true
            }
        }
    }
    // MARK: - Segues

    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "showReport" {
            if let index = tableView.indexPathForSelectedRow {
                let report: Event = fetchedResultsController.object(at: index)
                if let navigationController = segue.destination as? UINavigationController {
                    if let controller: ReportViewController = navigationController.topViewController as? ReportViewController {
                            controller.navigationItem.title = report.filename!
                            controller.report = report
                            controller.navigationItem.leftBarButtonItem = splitViewController?.displayModeButtonItem
                            controller.navigationItem.leftItemsSupplementBackButton = true

                        reportViewController = controller
                    }
                }
            }
        }
    }

    override func viewWillTransition(to size: CGSize, with coordinator: UIViewControllerTransitionCoordinator) {
        if let view: UIView = welcomeView {
            UITableViewController.removeWelcome(view: view)
            coordinator.animate(alongsideTransition: nil, completion: {_ in
                self.welcomeView = UITableViewController.displayWelcome(onView: self.view)
            })
        }
    }

    func showHideWelcome() {
        if tableView.numberOfRows(inSection: 0) == 0 {
            if let view = welcomeView {
                UITableViewController.removeWelcome(view: view)
            }
            welcomeView = UITableViewController.displayWelcome(onView: self.view)
        } else if let view: UIView = welcomeView {
            UITableViewController.removeWelcome(view: view)
            welcomeView = nil
        }
    }

    // MARK: - Table View

    override func numberOfSections(in tableView: UITableView) -> Int {
        return fetchedResultsController.sections?.count ?? 0
    }

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return fetchedResultsController.sections![section].numberOfObjects
    }

    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "Cell", for: indexPath)
        let event = fetchedResultsController.object(at: indexPath)

        configureCell(cell, withEvent: event)
        return cell
    }

    override func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the specified item to be editable.
        return true
    }

    override func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCell.EditingStyle, forRowAt indexPath: IndexPath) {

        // Close deleted report
        if editingStyle == .delete {
            if let report: Event = reportViewController?.report {
                if fetchedResultsController.object(at: indexPath).timestamp == report.timestamp {
                        reportViewController?.report = nil
                }
            }

            let context = fetchedResultsController.managedObjectContext
            context.delete(fetchedResultsController.object(at: indexPath))

            do {
                try context.save()
            } catch {
                let nserror = error as NSError

                view.makeToast("ERROR: \(nserror), \(nserror.userInfo) when trying to save reports", duration: 5.0, position: .top)
            }
        }
    }

    func configureCell(_ cell: UITableViewCell, withEvent event: Event) {
        cell.textLabel!.text = event.filename!
    }

    // MARK: - Fetched results controller

    var fetchedResultsController: NSFetchedResultsController<Event> {
        if _fetchedResultsController != nil {
            return _fetchedResultsController!
        }

        // Set the batch size to a suitable number.
        fetchRequest.fetchBatchSize = 20

        // Edit the sort key as appropriate.
        let sortDescriptor = NSSortDescriptor(key: "timestamp", ascending: true)

        fetchRequest.sortDescriptors = [sortDescriptor]

        let aFetchedResultsController = NSFetchedResultsController(fetchRequest: fetchRequest, managedObjectContext: self.managedObjectContext!, sectionNameKeyPath: nil, cacheName: "ReportsList")
        aFetchedResultsController.delegate = self
        _fetchedResultsController = aFetchedResultsController

        do {
            try _fetchedResultsController!.performFetch()
        } catch {
             let nserror = error as NSError

            view.makeToast("ERROR: \(nserror), \(nserror.userInfo) when trying to get saved reports", duration: 5.0, position: .top)
        }

        return _fetchedResultsController!
    }
    var _fetchedResultsController: NSFetchedResultsController<Event>? = nil

    func controllerWillChangeContent(_ controller: NSFetchedResultsController<NSFetchRequestResult>) {
        tableView.beginUpdates()
    }

    func controller(_ controller: NSFetchedResultsController<NSFetchRequestResult>, didChange sectionInfo: NSFetchedResultsSectionInfo, atSectionIndex sectionIndex: Int, for type: NSFetchedResultsChangeType) {
        switch type {
            case .insert:
                tableView.insertSections(IndexSet(integer: sectionIndex), with: .fade)
            case .delete:
                tableView.deleteSections(IndexSet(integer: sectionIndex), with: .fade)
            default:
                return
        }
    }

    func controller(_ controller: NSFetchedResultsController<NSFetchRequestResult>, didChange anObject: Any, at indexPath: IndexPath?, for type: NSFetchedResultsChangeType, newIndexPath: IndexPath?) {
        switch type {
            case .insert:
                tableView.insertRows(at: [newIndexPath!], with: .fade)
            case .delete:
                tableView.deleteRows(at: [indexPath!], with: .fade)
            case .update:
                configureCell(tableView.cellForRow(at: indexPath!)!, withEvent: anObject as! Event)
            case .move:
                configureCell(tableView.cellForRow(at: indexPath!)!, withEvent: anObject as! Event)
                tableView.moveRow(at: indexPath!, to: newIndexPath!)
            @unknown default:
                return
        }
    }

    func controllerDidChangeContent(_ controller: NSFetchedResultsController<NSFetchRequestResult>) {
        tableView.endUpdates()
        tableView.reloadData()

        showHideWelcome()
    }

    // MARK: subscription

    @objc func subscriptionStateChanged(_ notification: Notification) {
        if SubscriptionManager.shared.subscriptionActive {
            subscriptionActive()
        }
    }

    open func subscriptionActive() {
        if Core.shared.darkMode {
            enableDarkMode()
        }

        NotificationCenter.default.addObserver(self, selector: #selector(darkModeEnabled(_:)), name: .darkModeEnabled, object: nil)

        NotificationCenter.default.addObserver(self, selector: #selector(darkModeDisabled(_:)), name: .darkModeDisabled, object: nil)
    }

    // MARK: - Theme

    @objc func darkModeEnabled(_ notification: Notification) {
        enableDarkMode()

        if let view: UIView = welcomeView {
            UITableViewController.removeWelcome(view: view)
            welcomeView = UITableViewController.displayWelcome(onView: self.view)
        }
        tableView.reloadData()
    }

    @objc func darkModeDisabled(_ notification: Notification) {
        disableDarkMode()

        if let view: UIView = welcomeView {
            UITableViewController.removeWelcome(view: view)
            welcomeView = UITableViewController.displayWelcome(onView: self.view)
        }
        tableView.reloadData()
    }

    override func tableView(_ tableView: UITableView, willDisplay cell: UITableViewCell, forRowAt indexPath: IndexPath) {
        if SubscriptionManager.shared.subscriptionActive && Core.shared.darkMode {
            cell.backgroundColor = UIColor.darkGray
            cell.textLabel?.textColor = UIColor.white
        } else {
            cell.backgroundColor = UIColor.white
            cell.textLabel?.textColor = UIColor.black
        }
    }

    override func tableView(_ tableView: UITableView, viewForFooterInSection section: Int) -> UIView? {
        let rect = tableView.rectForFooter(inSection: section)
        if tableView.numberOfRows(inSection: section) == 0 {
            return UIView()
        }
        let clearButton = UIButton(type: .custom)
        clearButton.setTitle(NSLocalizedString("Clear list", tableName: "Core", comment: "").uppercased(), for: .normal)
        clearButton.addTarget(self, action: #selector(clearList(_:)), for: .touchUpInside)
        clearButton.setTitleColor(UIColor.gray, for: .normal)
        clearButton.frame = rect

        return clearButton
    }

    override func tableView(_ tableView: UITableView, heightForFooterInSection section: Int) -> CGFloat {
        return 40.0
    }

    @objc func clearList(_ sender: Any) {
        let context = self.fetchedResultsController.managedObjectContext
        let fetchRequest = NSFetchRequest<NSFetchRequestResult>(entityName: "Event")

        fetchRequest.returnsObjectsAsFaults = false

        do {
            let results = try context.fetch(fetchRequest)
            for object in results {
                if let objectData = object as? NSManagedObject {
                    context.delete(objectData)
                }
            }
            try context.save()
        }
        catch {
            let nserror = error as NSError
            self.view.makeToast("ERROR: \(nserror), \(nserror.userInfo) when trying to delete reports", duration: 5.0, position: .top)
        }
    }

    open func enableDarkMode() {
        view.backgroundColor = UIColor.darkGray
        navigationController?.navigationBar.barStyle = .black
    }

    open func disableDarkMode() {
        view.backgroundColor = UIColor.white
        navigationController?.navigationBar.barStyle = .default
    }
}
