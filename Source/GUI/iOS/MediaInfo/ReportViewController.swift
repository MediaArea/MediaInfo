/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

import UIKit
import Toast_Swift

class ReportViewController: UIViewController, UIDocumentPickerDelegate {
    @IBOutlet weak var reportView: UIWebView!
    @IBOutlet weak var viewsButton: UIBarButtonItem!
    @IBOutlet weak var exportButton: UIButton!

    let core: Core = Core()

    var currentView: String {
        get {
            return UserDefaults.standard.string(forKey: "View") ?? "HTML"
        }
        set(newValue) {
            if (newValue != currentView) {
                UserDefaults.standard.set(newValue, forKey: "View")
                configureView()
            }
        }
    }

    var report: Event? {
        didSet {
            configureView()
        }
    }

    func configureView() {
        // Update the view
        if report != nil {
            if let page: UIWebView = reportView {
                let reportText: String = core.convertReport(report: (NSKeyedUnarchiver.unarchiveObject(with: report!.report!) as! Array<MediaInfo_int8u>), format: currentView)

                if currentView == "HTML" {
                   page.loadHTMLString(reportText, baseURL: nil)
                } else {
                    page.loadHTMLString("<html><body><pre>" + reportText
                        .replacingOccurrences(of: "\t", with: "    ")
                        .replacingOccurrences(of: "<", with: "&lt;") +
                        "</pre></body></html>", baseURL: nil)
                }
            }

            if let button: UIButton = exportButton {
                button.isEnabled = true
            }
        } else {
            navigationItem.title = "Report"
            if let page: UIWebView = reportView,
               let url: URL = URL(string: "about:blank") {
                page.loadRequest(URLRequest(url: url))
            }

            if let button: UIButton = exportButton {
                button.isEnabled = false
            }
        }
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        // Toggle "tap to dismiss" functionality
        ToastManager.shared.isTapToDismissEnabled = true

        // Toggle queueing behavior
        ToastManager.shared.isQueueEnabled = true

        configureView()
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)

        // Bug, in iOS 11.2 navigation buttons remains faded after segue back
        if #available(iOS 11.2, *) {
            navigationController?.navigationBar.tintAdjustmentMode = .normal
            navigationController?.navigationBar.tintAdjustmentMode = .automatic
        }
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @IBAction func showViews(_ sender: Any) {
        performSegue(withIdentifier: "showViews", sender: self)
    }

    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "showViews" {
                let controller = segue.destination as! ViewsController
                controller.currentView = currentView
        }
    }

    @IBAction func unwindFromViewsController(_ sender: UIStoryboardSegue) {
        if let senderVC: ViewsController = sender.source as? ViewsController {
            if (currentView != senderVC.currentView) {
                currentView = senderVC.currentView
            }
        }
    }

    @IBAction func saveReport(_ sender: Any) {
        if let curReport = report {
            let content: String = core.convertReport(report: (NSKeyedUnarchiver.unarchiveObject(with: curReport.report!) as! Array<MediaInfo_int8u>), format: currentView, export: true)

            let fileManager: FileManager = FileManager.default
            let fileName: String = "\(curReport.filename!).\(currentView).\(core.getExtension(view: currentView))"
            let path: String = "\(NSTemporaryDirectory())/\(fileName)"

            var urlComponents = URLComponents()
            urlComponents.scheme = "file"
            urlComponents.path = path

            if let pathURL: URL = urlComponents.url,
                fileManager.createFile(atPath: path, contents: content.data(using: .utf8)) {
                let documentPicker: UIDocumentPickerViewController =  UIDocumentPickerViewController(url: pathURL, in: UIDocumentPickerMode.moveToService)
                documentPicker.delegate = self
                present(documentPicker, animated: true, completion: nil)
            } else {
                view.makeToast("ERROR: unable to export report", duration: 5.0, position: .top)
            }
        }
    }
}
