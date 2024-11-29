// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <filesystem>
#include <string>

#include <shlobj_core.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// C++/WinRT
#include <winrt/Windows.Foundation.Collections.h>

// Windows Implementation Libraries (WIL)
#include <wil/stl.h>
#include <wil/win32_helpers.h>

#endif //PCH_H
