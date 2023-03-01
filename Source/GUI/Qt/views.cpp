/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "views.h"
#include "translate.h"

QString nameView(ViewMode v) {
   switch(v) {
   case VIEW_EASY: return Tr("Easy");
       break;
   case VIEW_SHEET: return Tr("Sheet");
       break;
   case VIEW_TREE: return Tr("Tree");
       break;
   case VIEW_TEXT: return Tr("Text");
       break;
   case VIEW_HTML: return Tr("HTML");
       break;
   case VIEW_XML: return Tr("XML");
       break;
   case VIEW_JSON: return Tr("JSON");
       break;
   case VIEW_PBCORE: return Tr("PBCore 1.2");
       break;
   case VIEW_PBCORE2: return Tr("PBCore 2.0");
       break;
   case VIEW_MPEG7_Strict: return Tr("MPEG-7 (strict)");
       break;
   case VIEW_MPEG7_Relaxed: return Tr("MPEG-7 (relaxed)");
       break;
   case VIEW_MPEG7_Extended: return Tr("MPEG-7 (extended)");
       break;
   case VIEW_EBUCORE_1_5: return Tr("EBUCore 1.5");
       break;
   case VIEW_EBUCORE_1_6: return Tr("EBUCore 1.6");
       break;
   case VIEW_EBUCORE_1_8_ps: return Tr("EBUCore 1.8 parameter then segment");
       break;
   case VIEW_EBUCORE_1_8_sp: return Tr("EBUCore 1.8 segment then parameter");
       break;
   case VIEW_EBUCORE_1_8_ps_JSON: return Tr("EBUCore 1.8 parameter then segment (JSON output)");
       break;
   case VIEW_EBUCORE_1_8_sp_JSON: return Tr("EBUCore 1.8 segment then parameter (JSON output)");
       break;
   case VIEW_FIMS_1_1: return Tr("FIMS 1.1");
       break;
   case VIEW_FIMS_1_2: return Tr("FIMS 1.2");
       break;
   case VIEW_FIMS_1_3: return Tr("FIMS 1.3");
       break;
   case VIEW_NISO_Z39_87: return Tr("NISO Z39.87");
       break;
   case VIEW_CUSTOM: return Tr("Custom");
       break;
   default: return Tr("Unknown view");
       break;
   }
}
