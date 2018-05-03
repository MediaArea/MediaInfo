/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include <QApplication>
#include <QtGlobal>
#include <QtPlugin>
#include <iostream>
#include "mainwindow.h"

#include <ZenLib/Ztring.h>
#include <ZenLib/ZtringListList.h>

using namespace ZenLib;
#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

using namespace std;

int main(int argc, char *argv[])
{
    QStringList filesnames;
    int output = -1;
#if defined(_WIN32) && defined(WINAPI_FAMILY) && (WINAPI_FAMILY==WINAPI_FAMILY_APP) //UWP Application
    QApplication::setAttribute(Qt::AA_ImmediateWidgetCreation, true);
    QApplication a(argc, argv);
#else
    QApplication a(argc, argv);
    QStringList args = QCoreApplication::arguments();

    foreach(QString arg,args) {
        if(!arg.compare("--help",Qt::CaseInsensitive)||!arg.compare("-h",Qt::CaseInsensitive)) {
            cout << "Usage : mediainfo-gui [OPTIONS] [files]" << endl;
            cout << "Options : " << endl;
            cout << "-h,--help : show this help" << endl;
            cout << "--output=OUTPUT : select the view OUTPUT at launch" << endl;
            cout << "--version : show the version" << endl;
            return 0;
        } else if(!arg.compare("--version",Qt::CaseInsensitive)) {
            Core C;
            C.Menu_Help_Version();
            cout << wstring2QString(C.Text_Get()).toStdString() << endl;
            return 0;
        } else if(arg.startsWith("--output=",Qt::CaseInsensitive)) {
            arg.remove(0,9);
            int i=0;
            while(i<NB_VIEW) {
                if(arg.compare(nameView(ViewMode(i)),Qt::CaseInsensitive)==0) {
                    cout << arg.toStdString() << "view selected" << endl;
                    output=i;
                    break;
                }
                i++;
            }
            if(i<0) {
                cout << arg.toStdString() << " : unkown view" << endl;
            }
        } else {
            filesnames.append(arg);
        }
    }
#endif
    MainWindow w(filesnames,output);

    w.show();
    return a.exec();
}
