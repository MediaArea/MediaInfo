/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "views.h"
#include <QObject>

QString nameView(ViewMode v) {
   switch(v) {
   case VIEW_EASY: return QObject::tr("Easy");
       break;
   case VIEW_SHEET: return QObject::tr("Sheet");
       break;
   case VIEW_TREE: return QObject::tr("Tree");
       break;
   case VIEW_TEXT: return QObject::tr("Text");
       break;
   case VIEW_HTML: return QObject::tr("HTML");
       break;
   case VIEW_XML: return QObject::tr("XML");
       break;
   case VIEW_JSON: return QObject::tr("JSON");
       break;
   case VIEW_CSV: return QObject::tr("CSV");
       break;
   case VIEW_GRAPH: return QObject::tr("Graph");
       break;
   case VIEW_PBCORE: return QObject::tr("PBCore 1.2");
       break;
   case VIEW_PBCORE2: return QObject::tr("PBCore 2.0");
       break;
   case VIEW_MPEG7_Strict: return QObject::tr("MPEG-7 (strict)");
       break;
   case VIEW_MPEG7_Relaxed: return QObject::tr("MPEG-7 (relaxed)");
       break;
   case VIEW_MPEG7_Extended: return QObject::tr("MPEG-7 (extended)");
       break;
   case VIEW_EBUCORE_1_5: return QObject::tr("EBUCore 1.5");
       break;
   case VIEW_EBUCORE_1_6: return QObject::tr("EBUCore 1.6");
       break;
   case VIEW_EBUCORE_1_8_ps: return QObject::tr("EBUCore 1.8 parameter then segment");
       break;
   case VIEW_EBUCORE_1_8_sp: return QObject::tr("EBUCore 1.8 segment then parameter");
       break;
   case VIEW_EBUCORE_1_8_ps_JSON: return QObject::tr("EBUCore 1.8 parameter then segment (JSON output)");
       break;
   case VIEW_EBUCORE_1_8_sp_JSON: return QObject::tr("EBUCore 1.8 segment then parameter (JSON output)");
       break;
   case VIEW_FIMS_1_1: return QObject::tr("FIMS 1.1");
       break;
   case VIEW_FIMS_1_2: return QObject::tr("FIMS 1.2");
       break;
   case VIEW_FIMS_1_3: return QObject::tr("FIMS 1.3");
       break;
   case VIEW_NISO_Z39_87: return QObject::tr("NISO Z39.87");
       break;
   case VIEW_CUSTOM: return QObject::tr("Custom");
       break;
   default: return QObject::tr("Unknown view");
       break;
   }
}
