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
            cell.accessoryType = UITableViewCellAccessoryType.checkmark
        }
        cell.textLabel?.text = core.views[indexPath.row].desc

        return cell
    }

    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        currentView = core.views[indexPath.row].name
        performSegue(withIdentifier: "unwindFromViews", sender: self)
    }
}
