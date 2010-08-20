#ifndef EXPORT_H
#define EXPORT_H

#include <QtGui/QDialog>
#include <QtCore/QIODevice>

namespace Ui {
    class Export;
}

class Export : public QDialog {
    Q_OBJECT
public:
    Export(QString filename, QWidget *parent = 0);
    ~Export();
    int getExportMode();
    QString getFileName();
    QIODevice::OpenMode getOpenMode();
    bool isAdvancedChecked();

    static const int HTML_MODE=1;
    static const int TEXT_MODE=0;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Export *ui;

private slots:
    void on_toolButton_pressed();
};

#endif // EXPORT_H
