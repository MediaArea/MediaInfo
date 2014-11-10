/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
    VIEW_PBCORE2,
    VIEW_CUSTOM,
    NB_VIEW
} ViewMode;

QString nameView(ViewMode v);

#endif // VIEWS_H
