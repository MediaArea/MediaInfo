#ifndef SHEETVIEW_H
#define SHEETVIEW_H

#include <QtGui\QFrame>
#include "Common/Core.h"

namespace Ui {
    class SheetView;
}

class SheetView : public QFrame {
    Q_OBJECT
public:
    SheetView(Core* C, QWidget *parent = 0);
    ~SheetView();

protected:
    void changeEvent(QEvent *e);

private:
    Core* C;
    void refreshDisplay();
    Ui::SheetView *ui;
};

#endif // SHEETVIEW_H
