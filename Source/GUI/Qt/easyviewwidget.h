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

#ifndef EASYVIEWWIDGET_H
#define EASYVIEWWIDGET_H

#include <QtGui/QWidget>
#include "Common/Core.h"
#include <QtGui/QGroupBox>
#include <QtCore/QDebug>

class EasyViewWidget : public QWidget
{
Q_OBJECT
public:
    explicit EasyViewWidget(Core* C, QWidget *parent = 0);

private:
    void refreshDisplay();
    QGroupBox* createBox(stream_t StreamKind, int StreamPos);
    QString Tags_Get_General();
    size_t Lines_Count_Get(stream_t StreamKind);
    size_t Boxes_Count_Get(size_t StreamKind);
    QString Title_Get(stream_t StreamKind);
    QVector<QWidget *> Boxes;
    Core* C;
    int FilePos;

signals:

public slots:
    void changeFilePos(int newFilePos);

};

#endif // EASYVIEWWIDGET_H
