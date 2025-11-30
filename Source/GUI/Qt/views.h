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
    VIEW_JSON,
    VIEW_CSV,
    VIEW_GRAPH,
    VIEW_PBCORE,
    VIEW_PBCORE2,
    VIEW_MPEG7_Strict,
    VIEW_MPEG7_Relaxed,
    VIEW_MPEG7_Extended,
    VIEW_EBUCORE_1_5,
    VIEW_EBUCORE_1_6,
    VIEW_EBUCORE_1_8_ps,
    VIEW_EBUCORE_1_8_sp,
    VIEW_EBUCORE_1_8_ps_JSON,
    VIEW_EBUCORE_1_8_sp_JSON,
    VIEW_FIMS_1_1,
    VIEW_FIMS_1_2,
    VIEW_FIMS_1_3,
    VIEW_NISO_Z39_87,
    VIEW_CUSTOM,
    NB_VIEW
} ViewMode;

QString nameView(ViewMode v);

#endif // VIEWS_H
