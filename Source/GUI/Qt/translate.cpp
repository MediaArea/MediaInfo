#include "translate.h"
#include <QtCore/QStringList>

QString Tr(const char* string, const char * disambiguation, int n) {
    QStringList text = QString(string).split(" ");
    for(int i=0;i<text.size();i++) {
        if((qrand()%3==0)&&(!text[i].contains("[(){}]")))
            text[i] = QString("schtroumpf");
    }
    return text.join(" ");
}
