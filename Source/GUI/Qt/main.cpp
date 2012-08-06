// GUI of MediaInfo
// Copyright (C) 2011-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include <iostream>
#include "mainwindow.h"


#include <ZenLib/Ztring.h>
#include <ZenLib/ZtringListList.h>
using namespace ZenLib;
#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

#ifdef _WIN32
    Q_IMPORT_PLUGIN(qsvg)
#endif //_WIN32

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args = QCoreApplication::arguments();
    bool no_gui=false;
    QStringList filesnames;
    int output=-1;
    foreach(QString arg,args) {
        if(!arg.compare("--help",Qt::CaseInsensitive)||!arg.compare("-h",Qt::CaseInsensitive)) {
            cout << "Usage : mediainfo-gui [OPTIONS] [files]" << endl;
            cout << "Options : " << endl;
            cout << "-h,--help : show this help" << endl;
            cout << "--output=OUTPUT : select the view OUTPUT at launch" << endl;
            cout << "--version : show the version" << endl;
            no_gui=true;
        } else if(!arg.compare("--version",Qt::CaseInsensitive)) {
            Core C;
            C.Menu_Help_Version();
            cout << wstring2QString(C.Text_Get()).toStdString() << endl;
            no_gui=true;
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
    if(!no_gui) {
        MainWindow w(filesnames,output);
        w.show();
        return a.exec();
    }
}
