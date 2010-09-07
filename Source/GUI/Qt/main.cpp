#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include "mainwindow.h"

#ifdef _WIN32
	Q_IMPORT_PLUGIN(qsvg)
#endif //_WIN32
 
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
