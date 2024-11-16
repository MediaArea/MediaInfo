/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "graphplugin.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <QApplication>

#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

using namespace ZenLib;

QString Generate_Graph_HTML(Core *C, QSettings *settings) {

    C->Menu_Option_Preferences_Option(__T("Graph_Adm_ShowTrackUIDs"), settings->value("Graph_Adm_ShowTrackUIDs",false).toBool() ? __T("1") : __T("0"));
    C->Menu_Option_Preferences_Option(__T("Graph_Adm_ShowChannelFormats"), settings->value("Graph_Adm_ShowChannelFormats",false).toBool() ? __T("1") : __T("0"));
    C->Menu_Option_Preferences_Inform(__T("Graph_Svg"));

    Ztring S1 = Ztring();
    Ztring InstallFolder =
        QString2wstring(QCoreApplication::applicationDirPath());
    Ztring State = C->Menu_Option_Preferences_Option(
        __T("Info_Graph_Svg_Plugin_State"), __T(""));
    if (State == __T("1")) {
        for (size_t Pos = 0; Pos < C->Count_Get(); Pos++) {
            Ztring Svg = C->MI->Inform(Pos);
            size_t Pos2 = Svg.find(__T("<svg"));
            if (Pos2 != std::string::npos)
                Svg = Svg.substr(Pos);
            S1 += (Pos2 ? __T("<br/>") : __T("")) + Svg;
        }

        if (File::Exists(InstallFolder + __T("\\Plugin\\Graph\\Template.html"))) {
            File F(InstallFolder + __T("\\Plugin\\Graph\\Template.html"));
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
