// GUI of MediaInfo
// Copyright (C) 2011-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef VIEWS_H
#define VIEWS_H
#include <QtCore/QString>
#include <QtCore/QDebug>

typedef enum {
    VIEW_EASY=0,
    VIEW_SHEET,
    VIEW_TREE,
    VIEW_TEXT,
    VIEW_HTML,
    VIEW_XML,
    VIEW_MPEG7,
    VIEW_PBCORE,
    VIEW_CUSTOM,
    NB_VIEW
} ViewMode;

QString nameView(ViewMode v);

#endif // VIEWS_H
