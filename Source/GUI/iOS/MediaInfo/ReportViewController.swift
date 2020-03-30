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
        var bgColor = "#FFFFFF"
        var fgColor = "#000000"
        if SubscriptionManager.shared.subscriptionActive && Core.shared.darkMode {
            bgColor = "#000000"
            if let bgComponents = UIColor.darkGray.cgColor.components, bgComponents.count > 0 {
                let r = Float(bgComponents[0])
                let g = Float(bgComponents.count > 2 ? bgComponents[1] : bgComponents[0])
                let b = Float(bgComponents.count > 2 ? bgComponents[2] : bgComponents[0])

                bgColor = String(format: "#%02lX%02lX%02lX", lroundf(r * 255), lroundf(g * 255), lroundf(b * 255))
            }

            fgColor = "#FFFFFF"
            if let fgComponents = UIColor.white.cgColor.components, fgComponents.count > 0 {
                let r = Float(fgComponents[0])
                let g = Float(fgComponents.count > 2 ? fgComponents[1] : fgComponents[0])
                let b = Float(fgComponents.count > 2 ? fgComponents[2] : fgComponents[0])

                fgColor = String(format: "#%02lX%02lX%02lX", lroundf(r * 255), lroundf(g * 255), lroundf(b * 255))
            }
        }

        if report != nil {
            if let page: UIWebView = reportView {
                let reportText: String = core.convertReport(report: (NSKeyedUnarchiver.unarchiveObject(with: report!.report!) as! Array<MediaInfo_int8u>), format: currentView)

                var html: String
                if currentView == "HTML" {
                   html = reportText
                } else {
                    html = "<html><body><pre>" + reportText
                        .replacingOccurrences(of: "\t", with: "    ")
                        .replacingOccurrences(of: "<", with: "&lt;") +
                        "</pre></body></html>"
                }

                html = html.replacingOccurrences(of: "<body>", with: "<body style=\"background-color: \(bgColor); color: \(fgColor)\">")

                page.loadHTMLString(html, baseURL: nil)
            }

            if let button: UIButton = exportButton {
                button.isEnabled = true
            }
        } else {
            navigationItem.title = NSLocalizedString("Report", tableName: "Core", comment: "")
            if let page: UIWebView = reportView {
                page.loadHTMLString("<html><body style=\"background-color: \(bgColor); color: \(fgColor)\"></body></html>", baseURL: nil)
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

        if SubscriptionManager.shared.subscriptionActive {
            subscriptionActive()
        }

        NotificationCenter.default.addObserver(self, selector: #selector(subscriptionStateChanged(_:)), name: .subscriptionStateChanged, object: nil)

        configureView()
    }

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
        configureView()
    }

    @objc func darkModeDisabled(_ notification: Notification) {
        disableDarkMode()
        configureView()
    }

    open func enableDarkMode() {
        self.view.backgroundColor = UIColor.darkGray
        self.navigationController?.navigationBar.barStyle = .black
    }

    open func disableDarkMode() {
        self.view.backgroundColor = UIColor.white
        self.navigationController?.navigationBar.barStyle = .default
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
