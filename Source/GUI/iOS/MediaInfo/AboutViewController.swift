/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

import UIKit

class AboutViewController: UIViewController {
    @IBOutlet weak var aboutText: UILabel!
    @IBOutlet weak var logoImage: UIImageView!

    override func viewDidLoad() {
        super.viewDidLoad()

        if let iconsDictionary = Bundle.main.infoDictionary?["CFBundleIcons"] as? [String:Any],
           let primaryIconsDictionary = iconsDictionary["CFBundlePrimaryIcon"] as? [String:Any],
           let iconFiles = primaryIconsDictionary["CFBundleIconFiles"] as? [String],
           let lastIcon = iconFiles.last {
            logoImage.image = UIImage(named: lastIcon)
        }

        let miVersion: String = Bundle.main.object(forInfoDictionaryKey: "CFBundleShortVersionString") as? String ?? ""
        let milVersion: String = Core().version
        aboutText.text = """
                         MediaInfo v\(miVersion) using MediaInfoLib v\(milVersion)\n
                         Copyright (c) 2002-2018 MediaArea.net SARL. All rights reserved.\n\n
                         MediaInfo provides easy access to technical and tag information about video and audio files.\n
                         It is open-source software, which means that it is free of charge to the end user and developers have freedom to study, to improve and to redistribute the program (BSD license).
                         """

        aboutText.sizeToFit()
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
}
