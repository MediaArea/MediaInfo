/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QtGlobal>
#include <QtPlugin>
#include <iostream>
#include "mainwindow.h"

#include <ZenLib/Ztring.h>
#include <ZenLib/ZtringListList.h>

#include "Common/Preferences.h"

using namespace ZenLib;
using namespace std;

int main(int argc, char *argv[])
{
    QStringList filesnames;
    int output = -1;

    QCoreApplication::setApplicationName("mediainfo-gui");
    QCoreApplication::setApplicationVersion(QString().fromStdString(Ztring(MediaInfo_Version_GUI).To_UTF8()));

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
#if defined(_WIN32) && defined(WINAPI_FAMILY) && (WINAPI_FAMILY==WINAPI_FAMILY_APP) //UWP Application
    QApplication::setAttribute(Qt::AA_ImmediateWidgetCreation, true);
    QApplication a(argc, argv);
#else
    QApplication a(argc, argv);
    QCommandLineParser parser;

    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("files", "Files to analyze.");

    QCommandLineOption outputOption(QStringList() << "o" << "output", "select the view OUTPUT at launch.", "OUTPUT");
    parser.addOption(outputOption);

    parser.process(a);

    if(!parser.value(outputOption).isEmpty())
    {
        int i=0;
        while(i<NB_VIEW)
        {
            if(parser.value(outputOption).compare(nameView(ViewMode(i)), Qt::CaseInsensitive)==0)
            {
                cout << parser.value(outputOption).toStdString() << " view selected" << endl;
                output=i;
                break;
            }
            i++;
        }
        if(i>=NB_VIEW) {
            cout << "Unknown view: " << parser.value(outputOption).toStdString() << endl;
        }
    }

    filesnames = parser.positionalArguments();
#endif
    MainWindow w(filesnames,output);

    w.show();
    return a.exec();
}
