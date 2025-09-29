/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include <print>
#include "PreRelease/Language_Others.h"
#include "PreRelease/Language_All.h"

auto main() -> int {
    std::println("Update Language.csv from other language files: {}", Language_Others_Run().To_UTF8().c_str());
    std::println("Create other language files from Language.csv: {}", Language_All_Run().To_UTF8().c_str());
}
