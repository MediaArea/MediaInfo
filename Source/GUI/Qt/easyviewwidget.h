/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef EASYVIEWWIDGET_H
#define EASYVIEWWIDGET_H

#include <QWidget>
#include "Common/Core.h"
#include <QGroupBox>
#include <QDebug>

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
