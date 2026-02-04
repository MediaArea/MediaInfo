/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef MEDIAINFO_HTML_NO

#include "htmlwidget.h"
#include "ZenLib/Ztring.h"
#include <QApplication>
#include <QComboBox>
#include <QDir>
#include <QStyleHints>
#include <QVBoxLayout>

#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

using namespace ZenLib;

HTMLViewWidget::HTMLViewWidget(Core *C, QSettings *settings, QWidget *parent)
    : QWidget(parent), C(C), FilePos(0) {

    if (C->Kind_Get() == Core::Kind_Graph_Svg) {
        C->Menu_Option_Preferences_Option(__T("Graph_Adm_ShowTrackUIDs"), settings->value("Graph_Adm_ShowTrackUIDs",false).toBool() ? __T("1") : __T("0"));
        C->Menu_Option_Preferences_Option(__T("Graph_Adm_ShowChannelFormats"), settings->value("Graph_Adm_ShowChannelFormats",false).toBool() ? __T("1") : __T("0"));

        QComboBox *fileChoice = new QComboBox(this);
        fileChoice->setMinimumContentsLength(1);
        for (size_t Pos = 0; Pos < C->Count_Get(); ++Pos)
            fileChoice->addItem(wstring2QString(C->Get(Pos, Stream_General, 0, __T("CompleteName"))));
        fileChoice->setCurrentIndex(FilePos);
        connect(fileChoice, SIGNAL(currentIndexChanged(int)), SLOT(changeFilePos(int)));

        webView = new WebViewWidget(this);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(fileChoice);
        layout->addWidget(webView);

        this->setLayout(layout);
    }
    else {
        webView = new WebViewWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(webView);
        this->setLayout(layout);
    }

#if defined(EDGE_WEBVIEW2_YES) && (QT_VERSION >= QT_VERSION_CHECK(6, 8, 0))
    webView->setPreferredColorScheme(qApp->styleHints()->colorScheme());
#endif

    tempFile.setFileTemplate(tempFile.fileTemplate() + ".html");
    refresh();
}

QString HTMLViewWidget::generateGraphHTML() {
    QString html;
    QString state{wstring2QString(C->Menu_Option_Preferences_Option(__T("Info_Graph_Svg_Plugin_State"), __T("")))};
    if (state == "1") {
        QString svg{wstring2QString(C->MI->Inform(FilePos))};
        auto svgBeginPos{svg.indexOf("<svg")};
        if (svgBeginPos != -1)
            svg = svg.mid(svgBeginPos);
        QFile templateFile(":templates/Plugin/Graph/Template.html");
        if (templateFile.exists()) {
            if (templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&templateFile);
                QString graphTemplate{in.readAll()};
                templateFile.close();
                if (graphTemplate.indexOf("@SVG@") != -1)
                    html = graphTemplate.replace("@SVG@", svg);
                else
                    html = "Invalid template";
            } else
                html = "Unable to load graph template";
        } else
            html = "Graph template not found";
    } else if (state == "0")
        html = "Graph plugin not installed";
    else
        html = state;

    return html;
}

void HTMLViewWidget::refresh() {
    QString HTML;
    if (C->Kind_Get() == Core::Kind_Graph_Svg)
        HTML = generateGraphHTML();
    else
        HTML = wstring2QString(C->Inform_Get());

    if (HTML.toUtf8().size() < 0.5e6)
        webView->setHtml(HTML);
    else {
        if (!tempFile.open())
            return;
        tempFile.resize(0);
        tempFile.write(HTML.toUtf8());
        webView->load(QUrl::fromLocalFile(tempFile.fileName()));
    }
}

void HTMLViewWidget::changeFilePos(int newFilePos) {
    FilePos = newFilePos;
    refresh();
}

#endif // MEDIAINFO_HTML_NO
