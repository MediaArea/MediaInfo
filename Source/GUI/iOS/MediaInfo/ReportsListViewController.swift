/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

import UIKit
import CoreData
import MobileCoreServices

import Toast_Swift

extension UITableViewController {
    class func displayWelcome(onView: UIView) -> UIView {
        let welcomeView = UILabel.init(frame: onView.bounds)
        welcomeView.text = "You must at least open 1 file."

        welcomeView.textColor = UIColor.init(red: 0, green: 0, blue: 0, alpha: 1)
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
class ReportsListViewController: UITableViewController, NSFetchedResultsControllerDelegate, UIDocumentPickerDelegate {

    var reportViewController: ReportViewController? = nil
    var managedObjectContext: NSManagedObjectContext? = nil
    var selectedReport: Event? = nil
    var welcomeView: UIView? = nil

    let core: Core = Core()

    override func viewDidLoad() {
        super.viewDidLoad()

        let aboutButton = UIBarButtonItem.init(title: "About", style: .plain, target: self, action: #selector(showAbout(_:)))
        navigationItem.leftBarButtonItem = aboutButton

        let addButton = UIBarButtonItem(title: "Open", style: .plain, target: self, action: #selector(insertReport(_:)))
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

        showHideWelcome()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @objc func showAbout(_ sender: Any) {
       performSegue(withIdentifier: "showAbout", sender: self)
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
            if  welcomeView == nil {
                welcomeView = UITableViewController.displayWelcome(onView: self.view)
            }
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

    override func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCellEditingStyle, forRowAt indexPath: IndexPath) {

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

   /* override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        selectedReport = fetchedResultsController.object(at: indexPath)
     //   performSegue(withIdentifier: "showReport2", sender: self)
    }*/

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
        }
    }

    func controllerDidChangeContent(_ controller: NSFetchedResultsController<NSFetchRequestResult>) {
        tableView.endUpdates()

        showHideWelcome()
    }
}
