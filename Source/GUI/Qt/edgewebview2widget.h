/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

// webview2widget.h
// From code generated with Qwen2.5-Max

#ifndef EDGEWEBVIEW2WIDGET_H
#define EDGEWEBVIEW2WIDGET_H

#include <QQueue>
#include <QString>
#include <QUrl>
#include <QWidget>
#include <WebView2.h>
#include <windows.h>
#include <wrl.h>

class WebView2Widget : public QWidget {
    Q_OBJECT

public:
    explicit WebView2Widget(QWidget *parent = nullptr);
    ~WebView2Widget();

    // Public API: Load HTML content
    void setHtml(const QString &html);

    // Public API: Load a URL
    void load(const QUrl &url);
    void load(const QString &url);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    HRESULT InitializeWebView();
    void ProcessPendingRequests();

    HWND m_hwndHost;
    Microsoft::WRL::ComPtr<ICoreWebView2Controller> m_webviewController;
    Microsoft::WRL::ComPtr<ICoreWebView2> m_webview;

    bool m_isInitialized;          // Flag to track WebView2 initialization status
    QQueue<QString> m_pendingHtml; // Queue for pending HTML content
    QQueue<QUrl> m_pendingUrls;    // Queue for pending URLs
};

#endif // EDGEWEBVIEW2WIDGET_H
