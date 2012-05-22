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

#include "translate.h"
#include <QtCore/QStringList>

QString Tr(const char* string, const char * disambiguation, int n) {
    QStringList text = QString(string).split(" ");
    /* TODO
	for(int i=0;i<text.size();i++) {
        if(text[i]=="%n")
            text[i] = QString::number(n);
        else if((qrand()%3==0)&&(!text[i].contains(QRegExp("[\,|\.|\(|\)|\{|\}]")))) {
            text[i] = text[i].toUpper();
        }
    }
	*/
    return text.join(" ");
}
