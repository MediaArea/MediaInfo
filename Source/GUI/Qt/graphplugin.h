/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef GRAPHPLUGIN_H
#define GRAPHPLUGIN_H

#ifndef MEDIAINFO_HTML_NO

#include "Common/Core.h"
#include <QSettings>
#include <QString>
#include <QWidget>

#ifdef EDGE_WEBVIEW2_YES
#include "webview2widget.h"
#define WebViewWidget WebView2Widget
#else
#include <QWebEngineView>
#define WebViewWidget QWebEngineView
#endif

class GraphViewWidget : public QWidget {
    Q_OBJECT
public:
    explicit GraphViewWidget(Core *C, QSettings *settings, QWidget *parent = nullptr);

private:
    QString Generate_Graph_HTML();
    void refresh();
    Core *C;
    int FilePos;
    WebViewWidget *webView;

signals:

private slots:
    void changeFilePos(int newFilePos);
};

#endif // MEDIAINFO_HTML_NO

#endif // GRAPHPLUGIN_H
