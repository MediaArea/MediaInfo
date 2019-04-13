/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

import UIKit

class ViewsController: UITableViewController {
    var core: Core = Core()
    var currentView: String = "HTML"

    override func viewDidLoad() {
        super.viewDidLoad()

        if SubscriptionManager.shared.subscriptionActive {
            subscriptionActive()
        }

        NotificationCenter.default.addObserver(self, selector: #selector(subscriptionStateChanged(_:)), name: .subscriptionStateChanged, object: nil)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    override func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return core.views.count
    }

    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell: UITableViewCell = tableView.dequeueReusableCell(withIdentifier: "ViewCell", for: indexPath)

        if(core.views[indexPath.row].name == currentView) {
            cell.accessoryType = UITableViewCell.AccessoryType.checkmark
        }
        cell.textLabel?.text = core.views[indexPath.row].desc

        return cell
    }

    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        currentView = core.views[indexPath.row].name
        performSegue(withIdentifier: "unwindFromViews", sender: self)
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
        self.tableView.reloadData()
    }

    @objc func darkModeDisabled(_ notification: Notification) {
        disableDarkMode()
        self.tableView.reloadData()
    }

    override func tableView(_ tableView: UITableView, willDisplay cell: UITableViewCell, forRowAt indexPath: IndexPath) {
        if SubscriptionManager.shared.subscriptionActive && core.darkMode {
            cell.backgroundColor = UIColor.darkGray
            cell.textLabel?.textColor = UIColor.white
        } else {
            cell.backgroundColor = UIColor.white
            cell.textLabel?.textColor = UIColor.black
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
