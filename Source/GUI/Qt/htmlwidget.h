/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef HTMLWIDGET_H
#define HTMLWIDGET_H

#ifndef MEDIAINFO_HTML_NO

#include "Common/Core.h"
#include <QSettings>
#include <QString>
#include <QTemporaryFile>
#include <QWidget>

#ifdef EDGE_WEBVIEW2_YES
#include "edgewebview2widget.h"
#define WebViewWidget WebView2Widget
#else
#include <QWebEngineView>
#define WebViewWidget QWebEngineView
#endif

class HTMLViewWidget : public QWidget {
    Q_OBJECT
public:
    explicit HTMLViewWidget(Core *C, QSettings *settings, QWidget *parent = nullptr);

private:
    QString generateGraphHTML();
    void refresh();
    Core *C;
    int FilePos;
    WebViewWidget *webView;
    QTemporaryFile tempFile;

signals:

private slots:
    void changeFilePos(int newFilePos);
};

#endif // MEDIAINFO_HTML_NO

#endif // HTMLWIDGET_H
