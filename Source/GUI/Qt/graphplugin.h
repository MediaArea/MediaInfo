/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef GRAPHPLUGIN_H
#define GRAPHPLUGIN_H

#include "Common/Core.h"
#include <QSettings>
#include <QString>

QString Generate_Graph_HTML(Core *C, QSettings *settings);

#endif // GRAPHPLUGIN_H
