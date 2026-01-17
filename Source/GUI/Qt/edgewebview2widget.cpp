/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

// webview2widget.cpp
// From code generated with Qwen2.5-Max and Gemini 2.5 Flash / 3 Pro

#include "edgewebview2widget.h"
#include <QDebug>
#include <QResizeEvent>
#include <QWindow>
#include <QTimer>
#include <Shlobj.h>
#include <WebView2.h>
#include <windows.h>
#include <wrl.h>

#pragma comment(lib, "User32.lib")

namespace {
    std::wstring GetUserDataFolder() {
        wchar_t appDataPath[MAX_PATH];
        SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, appDataPath);

        std::wstring userDataFolder = appDataPath;
        userDataFolder += L"\\mediainfo-gui\\WebView2";

        // Create the folder if it doesn't exist
        CreateDirectoryW(userDataFolder.c_str(), nullptr);

        return userDataFolder;
    }
}

WebView2Widget::WebView2Widget(QWidget *parent)
    : QWidget(parent), m_hwndHost(nullptr), m_preferredColorScheme(COREWEBVIEW2_PREFERRED_COLOR_SCHEME_AUTO), m_isInitialized(false) {
    // Set up the widget to host WebView2
    m_hwndHost = (HWND)this->winId();

    // Initialize WebView2
    HRESULT hr = InitializeWebView();
    if (FAILED(hr)) {
        qWarning() << "Failed to initialize WebView2";
    }
}

WebView2Widget::~WebView2Widget() {
    if (m_webviewController) {
        m_webviewController->Close();
    }
}

HRESULT WebView2Widget::InitializeWebView() {
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr, GetUserDataFolder().c_str(), nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment *env) -> HRESULT {
                                        if (result == S_OK) {
                                            env->CreateCoreWebView2Controller(
                                                m_hwndHost,
                                                Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                                    [this](HRESULT result, ICoreWebView2Controller *controller) -> HRESULT {
                                                        Q_UNUSED(result);
                                                        if (controller != nullptr) {
                                                            m_webviewController = controller;
                                                            m_webviewController->get_CoreWebView2(&m_webview);

                                                            RECT bounds;
                                                            GetClientRect(m_hwndHost, &bounds);
                                                            m_webviewController->put_Bounds(bounds);
                                                            m_webviewController->put_IsVisible(TRUE);

                                                            // Mark WebView2 as initialized
                                                            m_isInitialized = true;

                                                            // Disable drag and drop
                                                            Microsoft::WRL::ComPtr<ICoreWebView2Controller4> controller4;
                                                            auto hr = m_webviewController.As(&controller4);
                                                            if (SUCCEEDED(hr))
                                                                controller4->put_AllowExternalDrop(FALSE);

                                                            // Get ICoreWebView2Profile interface for setting preferred color scheme
                                                            Microsoft::WRL::ComPtr<ICoreWebView2_13> webView13;
                                                            if (SUCCEEDED(m_webview.As(&webView13))) {
                                                                if (SUCCEEDED(webView13->get_Profile(&m_webviewProfile))) {
                                                                    m_webviewProfile->put_PreferredColorScheme(m_preferredColorScheme);
                                                                }
                                                            }

                                                            // Use a single-shot timer to delay resizing until after the widget is properly laid out
                                                            QTimer::singleShot(0, this, [this]() {
                                                                resizeEvent(nullptr); // Trigger resizeEvent manually
                                                            });

                                                            // Process any pending requests
                                                            ProcessPendingRequests();
                                                        }
                                                        return S_OK;
                                                    }
                                                ).Get()
                                            );
                                        }
                                        return result;
                                    }
        ).Get()
    );

    return hr;
}

void WebView2Widget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if (m_webviewController) {
        RECT bounds;
        GetClientRect(m_hwndHost, &bounds);
        m_webviewController->put_Bounds(bounds);
    }
}

void WebView2Widget::setPreferredColorScheme(Qt::ColorScheme colorScheme) {
    switch(colorScheme) {
    case Qt::ColorScheme::Unknown:  m_preferredColorScheme = COREWEBVIEW2_PREFERRED_COLOR_SCHEME_AUTO;    break;
    case Qt::ColorScheme::Light:    m_preferredColorScheme = COREWEBVIEW2_PREFERRED_COLOR_SCHEME_LIGHT;   break;
    case Qt::ColorScheme::Dark:     m_preferredColorScheme = COREWEBVIEW2_PREFERRED_COLOR_SCHEME_DARK;    break;
    }

    if (m_webviewProfile)
        m_webviewProfile->put_PreferredColorScheme(m_preferredColorScheme);
}

void WebView2Widget::ProcessPendingRequests() {
    // Process pending HTML content
    while (!m_pendingHtml.isEmpty()) {
        QString html = m_pendingHtml.dequeue();
        std::wstring htmlWide = html.toStdWString();
        m_webview->NavigateToString(htmlWide.c_str());
    }

    // Process pending URLs
    while (!m_pendingUrls.isEmpty()) {
        QUrl url = m_pendingUrls.dequeue();
        std::wstring urlWide = url.toString().toStdWString();
        m_webview->Navigate(urlWide.c_str());
    }
}

void WebView2Widget::setHtml(const QString &html) {
    if (m_isInitialized && m_webview) {
        std::wstring htmlWide = html.toStdWString();
        m_webview->NavigateToString(htmlWide.c_str());
    } else {
        // Queue the HTML content if WebView2 is not yet initialized
        m_pendingHtml.enqueue(html);
    }
}

void WebView2Widget::load(const QUrl &url) {
    if (m_isInitialized && m_webview) {
        std::wstring urlWide = url.toString().toStdWString();
        m_webview->Navigate(urlWide.c_str());
    } else {
        // Queue the URL if WebView2 is not yet initialized
        m_pendingUrls.enqueue(url);
    }
}

void WebView2Widget::load(const QString &url) { load(QUrl(url)); }
