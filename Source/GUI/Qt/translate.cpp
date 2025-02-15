/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "translate.h"
#include <QHash>

QString nameLanguage(const QString& code) {
    // Create a QHash to store language codes and their corresponding language names
    QHash<QString, QString> languageMap = {
        {"ar", "العربية (ar)"},
        {"be", "Беларуская (be)"},
        {"bg", "Български (bg)"},
        {"ca", "Català (ca)"},
        {"cs", "Czech (cs)"},
        {"da", "Dansk (da)"},
        {"de", "Deutsch (de)"},
        {"en", "English (en)"},
        {"es", "Español (es)"},
        {"eu", "Euskara (eu)"},
        {"fa", "Persian (فارسی) (fa)"},
        {"fr", "Français (fr)"},
        {"gl", "Galego (gl)"},
        {"gr", "Ελληνικά (gr)"},
        {"hu", "magyar (hu)"},
        {"id", "Bahasa Indonesia (id)"},
        {"it", "Italiano (it)"},
        {"ja", "日本語 (ja)"},
        {"ko", "한국어 (ko)"},
        {"lt", "Lithuanian (lt)"},
        {"nl", "Nederlands (nl)"},
        {"pl", "Polski (pl)"},
        {"pt", "Português (Portugal) (pt)"},
        {"pt-BR", "Português (Brasil) (pt-BR)"},
        {"ro", "Romana (ro)"},
        {"ru", "Русский (ru)"},
        {"sk", "Slovak (sk)"},
        {"sq", "Shqip (sq)"},
        {"sv", "Svenska (sv)"},
        {"th", "ไทย (th)"},
        {"tr", "Türkçe (tr)"},
        {"uk", "Українська (uk)"},
        {"zh-CN", "简体中文 (zh-CN)"},
        {"zh-HK", "香港正體字 (zh-HK)"},
        {"zh-TW", "華語 (台灣) (zh-TW)"},
        {"hr", "Hrvatski (hr)"},
        {"hy", "Հայերեն (hy)"},
        {"ka", "ქართული (ka)"}
    };

    // Search the map for the language code
    if (languageMap.contains(code)) {
        return languageMap.value(code);
    } else {
        return code; // Handle invalid code
    }
}
