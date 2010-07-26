#ifndef VIEWS_H
#define VIEWS_H
#include <QtCore/QString>

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
