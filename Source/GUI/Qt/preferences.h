#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QtGui/QDialog>
#include <QtCore/QSettings>

namespace Ui {
    class Preferences;
}

class Preferences : public QDialog {
    Q_OBJECT
public:
    Preferences(QSettings* settings, QWidget *parent = 0);
    ~Preferences();
    void saveSettings();

protected:
    void changeEvent(QEvent *e);

private:
    QSettings* settings;
    Ui::Preferences *ui;

private slots:
    void on_showMenu_toggled(bool checked);
    void on_showToolbar_toggled(bool checked);
    void on_treeWidget_itemSelectionChanged();
};

#endif // PREFERENCES_H
