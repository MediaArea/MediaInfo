#include "views.h"

QString nameView(ViewMode v) {
   switch(v) {
   case VIEW_EASY: return "Easy";
       break;
   case VIEW_SHEET: return "Sheet";
       break;
   case VIEW_TREE: return "Tree";
       break;
   case VIEW_TEXT: return "Text";
       break;
   case VIEW_HTML: return "HTML";
       break;
   case VIEW_XML: return "XML";
       break;
   case VIEW_MPEG7: return "MPEG-7";
       break;
   case VIEW_PBCORE: return "PBCore 1.2";
       break;
   case VIEW_CUSTOM: return "Custom";
       break;
   default: return "Unknown view";
       break;
   }
}
