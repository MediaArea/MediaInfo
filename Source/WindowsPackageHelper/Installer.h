/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#pragma once
#include "pch.h"

namespace MediaInfo_SparsePackage::Installer
{
    extern "C" __declspec(dllexport) HRESULT __stdcall Install();
    extern "C" __declspec(dllexport) HRESULT __stdcall Uninstall();
}