/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

import UIKit

class AboutViewController: UIViewController {
    @IBOutlet weak var aboutText: UILabel!
    @IBOutlet weak var logoImage: UIImageView!

    @IBAction func close(_ sender: Any) {
        dismiss(animated: true, completion: nil)
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        navigationItem.title = "About MediaInfo";
        navigationItem.rightBarButtonItem = UIBarButtonItem(title: "Close", style: .plain, target: self, action: #selector(close(_:)))
        navigationController?.isNavigationBarHidden = false

        if SubscriptionManager.shared.subscriptionActive {
            subscriptionActive()
        }

        NotificationCenter.default.addObserver(self, selector: #selector(subscriptionStateChanged(_:)), name: .subscriptionStateChanged, object: nil)

        logoImage.image = UIImage(named: "AboutIcon")

        let miVersion: String = Bundle.main.object(forInfoDictionaryKey: "CFBundleShortVersionString") as? String ?? ""
        let milVersion: String = Core().version
        aboutText.text = """
                         MediaInfo v\(miVersion) using MediaInfoLib v\(milVersion)\n
                         Copyright (c) 2002-2019 MediaArea.net SARL. All rights reserved.\n\n
                         MediaInfo provides easy access to technical and tag information about video and audio files.\n
                         It is open-source software, which means that it is free of charge to the end user and developers have freedom to study, to improve and to redistribute the program (BSD license).
                         """

        aboutText.sizeToFit()
    }

    @objc func close() {
        navigationController?.dismiss(animated: true, completion: nil)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }

    @IBAction func openWebsite(_ sender: Any) {
        if let url: URL = URL(string: "https://mediaarea.Net/en/MediaInfo"),
            UIApplication.shared.canOpenURL(url) {
            UIApplication.shared.open(url, options: [:], completionHandler: nil)
        }
    }

    @IBAction func openDonate(_ sender: Any) {
        if let url: URL = URL(string: "https://mediaarea.Net/MediaInfo/en/Donate"),
            UIApplication.shared.canOpenURL(url) {
            UIApplication.shared.open(url, options: [:], completionHandler: nil)
        }
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

    @objc func darkModeEnabled(_ notification: Notification) {
        enableDarkMode()
    }

    @objc func darkModeDisabled(_ notification: Notification) {
        disableDarkMode()
    }

    open func enableDarkMode() {
        aboutText.textColor = UIColor.white
        logoImage.backgroundColor = UIColor.darkGray
        view.backgroundColor = UIColor.darkGray
        navigationController?.navigationBar.barStyle = .black
    }

    open func disableDarkMode() {
        aboutText.textColor = UIColor.black
        logoImage.backgroundColor = UIColor.white
        view.backgroundColor = UIColor.white
        navigationController?.navigationBar.barStyle = .default
    }
}
