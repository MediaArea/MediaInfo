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
   case VIEW_MPEG7: return Tr("MPEG-7");
       break;
   case VIEW_PBCORE: return Tr("PBCore 1.2");
       break;
   case VIEW_PBCORE2: return Tr("PBCore 2.0");
       break;
   case VIEW_CUSTOM: return Tr("Custom");
       break;
   default: return Tr("Unknown view");
       break;
   }
}
