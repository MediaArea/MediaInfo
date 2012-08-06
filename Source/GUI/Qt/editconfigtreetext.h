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

#ifndef EDITCONFIGTREETEXT_H
#define EDITCONFIGTREETEXT_H

#include "translate.h"
#include <QtGui/QDialog>
#include "Common/Core.h"
#include "configtreetext.h"
#include <QtCore/QDebug>

namespace Ui {
    class editConfigTreeText;
}

class EditConfigTreeText : public QDialog {
    Q_OBJECT
public:
    EditConfigTreeText(ConfigTreeText* ctt, Core* C, QWidget *parent = 0);
    ~EditConfigTreeText();
    void apply();

protected:
    void changeEvent(QEvent *e);
    void fillToolBox();

private:
    Ui::editConfigTreeText *ui;
    Core* C;
    ConfigTreeText* ctt;
};

#endif // EDITCONFIGTREETEXT_H
