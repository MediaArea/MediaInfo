/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef MEDIAINFO_HTML_NO

#include "graphplugin.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <QApplication>
#include <QComboBox>
#include <QDir>
#include <QVBoxLayout>

#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

using namespace ZenLib;

GraphViewWidget::GraphViewWidget(Core *C, QSettings *settings, QWidget *parent)
    : QWidget(parent), C(C), FilePos(0) {

    C->Menu_Option_Preferences_Option(__T("Graph_Adm_ShowTrackUIDs"), settings->value("Graph_Adm_ShowTrackUIDs",false).toBool() ? __T("1") : __T("0"));
    C->Menu_Option_Preferences_Option(__T("Graph_Adm_ShowChannelFormats"), settings->value("Graph_Adm_ShowChannelFormats",false).toBool() ? __T("1") : __T("0"));
    C->Menu_Option_Preferences_Inform(__T("Graph_Svg"));

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

    tempFile.setFileTemplate(tempFile.fileTemplate() + ".html");
    refresh();
}

QString GraphViewWidget::Generate_Graph_HTML() {
    Ztring S1 = Ztring();
    Ztring InstallFolder = QString2wstring(QCoreApplication::applicationDirPath());
    Ztring State = C->Menu_Option_Preferences_Option(__T("Info_Graph_Svg_Plugin_State"), __T(""));
    if (State == __T("1")) {
        Ztring Svg = C->MI->Inform(FilePos);
        size_t SvgBeginPos = Svg.find(__T("<svg"));
        if (SvgBeginPos != std::string::npos)
            S1 = Svg.substr(SvgBeginPos);
        QString template_rel_path = "/Plugin/Graph/Template.html";
        if (File::Exists(InstallFolder + QString2wstring(QDir::toNativeSeparators(template_rel_path)))) {
            File F(InstallFolder + QString2wstring(QDir::toNativeSeparators(template_rel_path)));
            int8u *Buffer = new int8u[(size_t)F.Size_Get() + 1];
            size_t Count = F.Read(Buffer, (size_t)F.Size_Get());
            if (Count == ZenLib::Error) {
                S1 = __T("Unable to load graph template");
            } else {
                Buffer[Count] = (int8u)'\0';
                Ztring Template = Ztring().From_UTF8(reinterpret_cast<char *>(Buffer));
                if (Template.FindAndReplace(__T("@SVG@"), S1) == 0)
                    S1 = __T("Invalid template");
                else
                    S1 = Template;
            }
            delete[] Buffer;
        } else
            S1 = __T("Graph template not found");
    } else if (State == __T("0"))
        S1 = __T("Graph plugin not installed");
    else
        S1 = State;

    return wstring2QString(S1);
}

void GraphViewWidget::refresh() {
    QString graphHTML = Generate_Graph_HTML();
    if (graphHTML.toUtf8().size() < 0.5e6)
        webView->setHtml(graphHTML);
    else {
        if (!tempFile.open())
            return;
        tempFile.resize(0);
        tempFile.write(graphHTML.toUtf8());
        webView->load(QUrl::fromLocalFile(tempFile.fileName()));
    }
}

void GraphViewWidget::changeFilePos(int newFilePos) {
    FilePos = newFilePos;
    refresh();
}

#endif // MEDIAINFO_HTML_NO
