/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

import UIKit
import Foundation

import Toast_Swift

extension UIButton {
    func loadingIndicator(_ show: Bool) {
        let tag = 162411
        if show {
            self.isEnabled = false
            self.alpha = 0.5
            let indicator = UIActivityIndicatorView()
            let buttonHeight = self.bounds.size.height
            let buttonWidth = self.bounds.size.width
            indicator.center = CGPoint(x: buttonWidth/2, y: buttonHeight/2)
            indicator.tag = tag
            self.addSubview(indicator)
            indicator.startAnimating()
        } else {
            self.isEnabled = true
            self.alpha = 1.0
            if let indicator = self.viewWithTag(tag) as? UIActivityIndicatorView {
                indicator.stopAnimating()
                indicator.removeFromSuperview()
            }
        }
    }
}

protocol SubscribeResultDelegate {
    func showMessage(message: String?)
}

class SubscribeViewController: UIViewController {
    @IBOutlet weak var subscribeText: UILabel!
    @IBOutlet weak var statusText: UILabel!
    @IBOutlet weak var subscribeButton: UIButton!
    @IBOutlet weak var legalText: UILabel!

    var message: String? = nil
    var delegate: SubscribeResultDelegate? = nil
    var purchasing: Bool = false

    @IBAction func subscribe(_ sender: Any) {
        subscribeButton.loadingIndicator(true)
        purchasing = true
        SubscriptionManager.shared.purchase()
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)

        if let date = SubscriptionManager.shared.subscriptionEndDate {
            let formatter = DateFormatter()
            formatter.dateStyle = .short
            formatter.timeStyle = .short

            if date >= Date() {
                statusText.text = NSLocalizedString("Subscription active until %DATE%", tableName: "Core", comment: "").replacingOccurrences(of: "%DATE%", with: formatter.string(from: date))
                if let short = Calendar.current.date(byAdding: .day, value: 7, to: Date()), short >= date {
                    statusText.textColor = UIColor.orange
                } else {
                    statusText.textColor = UIColor.green
                }
                statusText.sizeToFit()
            } else {
                statusText.text = NSLocalizedString("Subscription expired since %DATE%", tableName: "Core", comment: "").replacingOccurrences(of: "%DATE%", with: formatter.string(from: date))
                statusText.textColor = UIColor.red
                statusText.sizeToFit()
            }
        }
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        navigationItem.title = NSLocalizedString("Subscribe", tableName: "Core", comment: "")
        navigationItem.rightBarButtonItem = UIBarButtonItem(title: NSLocalizedString("Close", tableName: "Core", comment: ""), style: .plain, target: self, action: #selector(close))
        navigationController?.isNavigationBarHidden = false

        subscribeButton.loadingIndicator(true)

        if SubscriptionManager.shared.subscriptionDetails == nil {
            NotificationCenter.default.addObserver(self, selector: #selector(subscriptionDetailsAviable(_:)), name: .subscriptionDetailsReady, object: nil)

            NotificationCenter.default.addObserver(self, selector: #selector(subscriptionDetailsAviable(_:)), name: .subscriptionDetailsUnaviable, object: nil)
            SubscriptionManager.shared.loadSubscription()
        } else {
            updateSubscriptionDetails()
        }

        NotificationCenter.default.addObserver(self, selector: #selector(purchaseFailed(_:)), name: .purchaseFailed, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(purchaseDeferred(_:)), name: .purchaseDeferred, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(purchaseSucceeded(_:)), name: .purchaseSucceeded, object: nil)
    }

    override func viewDidDisappear(_ animated: Bool) {
        delegate?.showMessage(message: message)
    }

    @objc func close() {
        navigationController?.dismiss(animated: true, completion: nil)
    }

    @objc func purchaseFailed(_ notification: Notification) {
        if (purchasing) {
            purchasing = false
            subscribeButton.loadingIndicator(false)
            view.makeToast(NSLocalizedString("Purchase canceled or failed", tableName: "Core", comment: ""), duration: 5.0, position: .top)
        }
    }

    @objc func purchaseDeferred(_ notification: Notification) {
        if (purchasing) {
            purchasing = false
            subscribeButton.loadingIndicator(false)
            view.makeToast(NSLocalizedString("Purchase waiting for parent approval", tableName: "Core", comment: ""), duration: 5.0, position: .top)
        }
    }

    @objc func purchaseSucceeded(_ notification: Notification) {
        if (purchasing) {
            purchasing = false
            subscribeButton.loadingIndicator(false)
            message = NSLocalizedString("Purchase succeeded, thanks for your support!", tableName: "Core", comment: "")
            close()
        }
    }

    @objc func subscriptionDetailsAviable(_ notification: Notification) {
        updateSubscriptionDetails()
    }

    open func updateSubscriptionDetails() {
        if let subscriptionDetails = SubscriptionManager.shared.subscriptionDetails {
            let formatter = NumberFormatter()
            formatter.numberStyle = .currency
            formatter.locale = subscriptionDetails.priceLocale

            if let price = formatter.string(from: subscriptionDetails.price) {
                subscribeButton.loadingIndicator(false)

                if SubscriptionManager.shared.subscriptionEndDate != nil {
                    subscribeButton.setTitle(NSLocalizedString("Renew subscription (%PRICE% for one year)", tableName: "Core", comment: "").replacingOccurrences(of: "%PRICE%", with: price), for: .normal)
                } else {
                    subscribeButton.setTitle(NSLocalizedString("Subscribe (%PRICE% for one year)", tableName: "Core", comment: "").replacingOccurrences(of: "%PRICE%", with: price), for: .normal)
                }

                legalText.text = """
                A \(price) purchase will be applied to your iTunes account on confirmation.
                If a subscription is already active one year will be added to your current subscription end date.
                """
                legalText.sizeToFit()
            } else {
                message = NSLocalizedString("Unable to retrieve subscription details.", tableName: "Core", comment: "")
                close()
            }
        } else {
            message = NSLocalizedString("Unable to retrieve subscription details.", tableName: "Core", comment: "")
            close()
        }
    }
}
