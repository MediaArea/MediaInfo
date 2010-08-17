#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QtGui/QDialog>
#include <QtCore/QSettings>
#include "Common/Core.h"

namespace Ui {
    class Preferences;
}

class Preferences : public QDialog {
    Q_OBJECT
public:
    Preferences(QSettings* settings, Core* C, QWidget *parent = 0);
    ~Preferences();
    void saveSettings();

protected:
    void changeEvent(QEvent *e);

private:
    void refreshDisplay();
    QSettings* settings;
    Ui::Preferences *ui;
    Core* C;

private slots:
    void on_pushButton_deleteSheet_clicked();
    void on_pushButton_newSheet_clicked();
    void on_pushButton_editSheet_clicked();
    void on_treeWidget_itemSelectionChanged();
};

#endif // PREFERENCES_H
