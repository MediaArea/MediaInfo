// GUI of MediaInfo
// Copyright (C) 2011-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
   case VIEW_CUSTOM: return Tr("Custom");
       break;
   default: return Tr("Unknown view");
       break;
   }
}
