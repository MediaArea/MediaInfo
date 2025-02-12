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
    @IBOutlet weak var lifetimeSubscribeButton: UIButton!
    @IBOutlet weak var restoreButton: UIButton!
    @IBOutlet weak var legalText: UILabel!

    var message: String? = nil
    var delegate: SubscribeResultDelegate? = nil
    var purchasing: Bool = false

    @IBAction func subscribe(_ sender: Any) {
        subscribeButton.loadingIndicator(true)
        purchasing = true
        SubscriptionManager.shared.purchase(product: SubscriptionManager.shared.subscriptionDetails)
    }

    @IBAction func lifetimeSubscribe(_ sender: Any) {
        lifetimeSubscribeButton.loadingIndicator(true)
        purchasing = true
        SubscriptionManager.shared.purchase(product: SubscriptionManager.shared.lifetimeSubscriptionDetails)
    }

    @IBAction func restore(_ sender: Any) {
        SubscriptionManager.shared.restore()
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)

        if SubscriptionManager.shared.isLifetime {
            statusText.text = NSLocalizedString("Lifetime subscription detected.", tableName: "Core", comment: "")
        }
        else if let date = SubscriptionManager.shared.subscriptionEndDate {
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
        lifetimeSubscribeButton.loadingIndicator(true)

        if SubscriptionManager.shared.subscriptionDetails == nil || SubscriptionManager.shared.lifetimeSubscriptionDetails == nil {
            NotificationCenter.default.addObserver(self, selector: #selector(subscriptionDetailsAviable(_:)), name: .subscriptionDetailsReady, object: nil)

            NotificationCenter.default.addObserver(self, selector: #selector(subscriptionDetailsAviable(_:)), name: .subscriptionDetailsUnaviable, object: nil)
            SubscriptionManager.shared.loadSubscription()
        } else {
            updateSubscriptionDetails()
        }

        if SubscriptionManager.shared.subscriptionActive {
            subscriptionActive()
        }

        NotificationCenter.default.addObserver(self, selector: #selector(subscriptionStateChanged(_:)), name: .subscriptionStateChanged, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(purchaseFailed(_:)), name: .purchaseFailed, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(purchaseDeferred(_:)), name: .purchaseDeferred, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(purchaseSucceeded(_:)), name: .purchaseSucceeded, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(purchaseRestored(_:)), name: .restoreFinished, object: nil)
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
            lifetimeSubscribeButton.loadingIndicator(false)
            view.makeToast(NSLocalizedString("Purchase canceled or failed", tableName: "Core", comment: ""), duration: 5.0, position: .top)
        }
    }

    @objc func purchaseDeferred(_ notification: Notification) {
        if (purchasing) {
            purchasing = false
            subscribeButton.loadingIndicator(false)
            lifetimeSubscribeButton.loadingIndicator(false)
            view.makeToast(NSLocalizedString("Purchase waiting for parent approval", tableName: "Core", comment: ""), duration: 5.0, position: .top)
        }
    }

    @objc func purchaseSucceeded(_ notification: Notification) {
        if (purchasing) {
            purchasing = false
            subscribeButton.loadingIndicator(false)
            lifetimeSubscribeButton.loadingIndicator(false)
            message = NSLocalizedString("Purchase succeeded, thanks for your support!", tableName: "Core", comment: "")
            close()
        }
    }

    @objc func purchaseRestored(_ notification: Notification) {
        if SubscriptionManager.shared.isLifetime {
            message = NSLocalizedString("Purchase restored", tableName: "Core", comment: "")
        }
        else {
            message = NSLocalizedString("No lifetime purchase found", tableName: "Core", comment: "")
        }
    }

    @objc func subscriptionDetailsAviable(_ notification: Notification) {
        updateSubscriptionDetails()
    }

    open func updateSubscriptionDetails() {
        if let subscriptionDetails = SubscriptionManager.shared.subscriptionDetails, let lifetimeSubscriptionDetails = SubscriptionManager.shared.lifetimeSubscriptionDetails {
            let formatter = NumberFormatter()
            formatter.numberStyle = .currency
            formatter.locale = subscriptionDetails.priceLocale

            if let price = formatter.string(from: subscriptionDetails.price) {
                subscribeButton.loadingIndicator(false)

                if SubscriptionManager.shared.subscriptionEndDate != nil {
                    subscribeButton.setTitle(NSLocalizedString("Renew subscription (%PRICE% for one year)", tableName: "Core", comment: "").replacingOccurrences(of: "%PRICE%", with: price), for: .normal)
                } else {
                    subscribeButton.setTitle(NSLocalizedString("Subscribe %PRICE% for one year", tableName: "Core", comment: "").replacingOccurrences(of: "%PRICE%", with: price), for: .normal)
                }

                formatter.locale = lifetimeSubscriptionDetails.priceLocale

                if let lifetimePrice = formatter.string(from: lifetimeSubscriptionDetails.price) {
                    lifetimeSubscribeButton.loadingIndicator(false)
                    lifetimeSubscribeButton.setTitle(NSLocalizedString("Lifetime subscription for %PRICE%", tableName: "Core", comment: "").replacingOccurrences(of: "%PRICE%", with: lifetimePrice), for: .normal)

                    legalText.text = """
                        A \(price) renewable or \(lifetimePrice) lifetime purchase will be applied to your iTunes account on confirmation.
                        For a renewable subscription, if a subscription is already active one year will be added to your current subscription end date.
                    """
                    legalText.sizeToFit()
                }

                if SubscriptionManager.shared.isLifetime {
                    subscribeButton.isEnabled = false
                    lifetimeSubscribeButton.isEnabled = false
                }
            } else {
                message = NSLocalizedString("Unable to retrieve subscription details.", tableName: "Core", comment: "")
                close()
            }
        } else {
            message = NSLocalizedString("Unable to retrieve subscription details.", tableName: "Core", comment: "")
            close()
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
        subscribeText.textColor = UIColor.white
        subscribeText.textColor = UIColor.white
        legalText.textColor = UIColor.white
        view.backgroundColor = UIColor.darkGray
        navigationController?.navigationBar.barStyle = .black
    }

    open func disableDarkMode() {
        subscribeText.textColor = UIColor.black
        subscribeText.textColor = UIColor.black
        legalText.textColor = UIColor.black
        view.backgroundColor = UIColor.white
        navigationController?.navigationBar.barStyle = .default
    }
}
