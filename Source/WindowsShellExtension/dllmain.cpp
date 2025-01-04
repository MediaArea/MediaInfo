/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

// dllmain.cpp : Defines the entry point for the DLL application.

// IExplorerCommand File Explorer shell extension for context menu entry

#include "pch.h"

#ifdef MEDIAINFO_QT
    #define SHELLEXT_GUID "dea3006d-451d-4f0f-a2b2-c3250cb255b4"
#else
    #define SHELLEXT_GUID "20669675-b281-4c4f-94fb-cb6fd3995545"
#endif // MEDIAINFO_QT

constexpr const wchar_t* menu_entry_title = L"MediaInfo";
constexpr const wchar_t* exe_filename = L"MediaInfo.exe";

BOOL APIENTRY DllMain(_In_ HMODULE hModule, _In_ DWORD  ul_reason_for_call, _In_opt_ LPVOID lpReserved) {
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

namespace {
    // Extracted from
    // https://source.chromium.org/chromium/chromium/src/+/main:base/command_line.cc;l=109-159

    std::wstring QuoteForCommandLineArg(_In_ const std::wstring& arg) {
        // We follow the quoting rules of CommandLineToArgvW.
        // http://msdn.microsoft.com/en-us/library/17w5ykft.aspx
        std::wstring quotable_chars(L" \\\"");
        if (arg.find_first_of(quotable_chars) == std::wstring::npos) {
            // No quoting necessary.
            return arg;
        }

        std::wstring out;
        out.push_back('"');
        for (size_t i = 0; i < arg.size(); ++i) {
            if (arg[i] == '\\') {
                // Find the extent of this run of backslashes.
                size_t start = i, end = start + 1;
                for (; end < arg.size() && arg[end] == '\\'; ++end) {}
                size_t backslash_count = end - start;

                // Backslashes are escapes only if the run is followed by a double quote.
                // Since we also will end the string with a double quote, we escape for
                // either a double quote or the end of the string.
                if (end == arg.size() || arg[end] == '"') {
                    // To quote, we need to output 2x as many backslashes.
                    backslash_count *= 2;
                }
                for (size_t j = 0; j < backslash_count; ++j)
                    out.push_back('\\');

                // Advance i to one before the end to balance i++ in loop.
                i = end - 1;
            }
            else if (arg[i] == '"') {
                out.push_back('\\');
                out.push_back('"');
            }
            else {
                out.push_back(arg[i]);
            }
        }
        out.push_back('"');

        return out;
    }

    // Extracted from
    // https://learn.microsoft.com/en-us/archive/msdn-magazine/2017/may/c-use-modern-c-to-access-the-windows-registry#reading-a-dword-value-from-the-registry
    DWORD RegGetDword(_In_ HKEY hKey, _In_ const std::wstring& subKey, _In_ const std::wstring& value) {
        DWORD data{};
        DWORD dataSize = sizeof(data);
        LONG retCode = RegGetValue(
            hKey,
            subKey.c_str(),
            value.c_str(),
            RRF_RT_REG_DWORD,
            nullptr,
            &data,
            &dataSize
        );
        if (retCode != ERROR_SUCCESS)
            throw std::runtime_error("Cannot read DWORD from registry.");
        return data;
    }

    // Adapted from
    // https://learn.microsoft.com/en-us/archive/msdn-magazine/2017/may/c-use-modern-c-to-access-the-windows-registry#reading-a-string-value-from-the-registry
    bool RegGetBool(_In_ HKEY hKey, _In_ const std::wstring& subKey, _In_ const std::wstring& value) {
        DWORD dataSize{};
        LONG retCode = RegGetValue(
            hKey,
            subKey.c_str(),
            value.c_str(),
            RRF_RT_REG_SZ,
            nullptr,
            nullptr,
            &dataSize
        );
        if (retCode != ERROR_SUCCESS)
            throw std::runtime_error("Cannot read string from registry.");
        std::wstring data;
        data.resize(dataSize / sizeof(wchar_t));
        retCode = ::RegGetValue(
            hKey,
            subKey.c_str(),
            value.c_str(),
            RRF_RT_REG_SZ,
            nullptr,
            &data[0],
            &dataSize
        );
        if (retCode != ERROR_SUCCESS)
            throw std::runtime_error("Cannot read string from registry.");
        DWORD stringLengthInWchars = dataSize / sizeof(wchar_t);
        --stringLengthInWchars; // Exclude the NUL written by the Win32 API
        data.resize(stringLengthInWchars);
        if (data.compare(L"true") == 0)
            return true;
        if (data.compare(L"false") == 0)
            return false;
        throw std::runtime_error("Not a boolean.");
    }

    // Function to check for supported file extensions
    bool IsSupportedFileExtension(_In_ const std::string& extension) {
        const std::vector<std::string> supported_extensions{
            ".264",
            ".3g2",
            ".3ga",
            ".3gp",
            ".3gpa",
            ".3gpp",
            ".aa3",
            ".aac",
            ".aacp",
            ".adts",
            ".ac3",
            ".act",
            ".aif",
            ".aifc",
            ".aiff",
            ".amr",
            ".ape",
            ".asf",
            ".at3",
            ".au",
            ".aud",
            ".aue",
            ".avi",
            ".avif",
            ".avs",
            ".bdmv",
            ".bmp",
            ".bms",
            ".braw",
            ".caf",
            ".clpi",
            ".dat",
            ".dde",
            ".divx",
            ".dpg",
            ".dff",
            ".dsd",
            ".dsf",
            ".dts",
            ".dtshd",
            ".dv",
            ".dvr",
            ".dvr-ms",
            ".eac3",
            ".evo",
            ".f4a",
            ".f4b",
            ".f4v",
            ".fla",
            ".flc",
            ".fli",
            ".flac",
            ".flv",
            ".gvi",
            ".gif",
            ".gis",
            ".h264",
            ".h3d",
            ".hdmov",
            ".heic",
            ".heif",
            ".iamf",
            ".ico",
            ".ifo",
            ".ism",
            ".isma",
            ".ismv",
            ".j2k",
            ".jp2",
            ".jpeg",
            ".jpg",
            ".jps",
            ".jxl",
            ".m1s",
            ".m1t",
            ".m1v",
            ".m2p",
            ".m2s",
            ".m2t",
            ".m2ts",
            ".m2v",
            ".m4a",
            ".m4b",
            ".m4v",
            ".mac",
            ".mk3d",
            ".mka",
            ".mks",
            ".mkv",
            ".mlp",
            ".mod",
            ".mov",
            ".mp+",
            ".mp2",
            ".mp3",
            ".mp4",
            ".mpc",
            ".mpd",
            ".mpe",
            ".mpeg",
            ".mpg",
            ".mpgv",
            ".mpgx",
            ".mpls",
            ".mpm",
            ".mpo",
            ".mpv",
            ".mts",
            ".mxf",
            ".oga",
            ".ogg",
            ".ogm",
            ".ogv",
            ".ogx",
            ".oma",
            ".opus",
            ".png",
            ".pns",
            ".qcp",
            ".qt",
            ".ra",
            ".rm",
            ".rmvb",
            ".shn",
            ".smv",
            ".spdif",
            ".spx",
            ".stl",
            ".swf",
            ".tak",
            ".thd",
            ".thd+ac3",
            ".tif",
            ".tiff",
            ".tmf",
            ".tp",
            ".trec",
            ".trp",
            ".ts",
            ".tta",
            ".ty",
            ".vob",
            ".vqf",
            ".vro",
            ".w64",
            ".wav",
            ".webm",
            ".webp",
            ".wma",
            ".wmv",
            ".wtv",
            ".wv",
            ".wvc",
            ".y4m"
        };
        std::string extension_lower{ extension };
        std::for_each(extension_lower.begin(), extension_lower.end(), [](char& c) { c = static_cast<char>(tolower(c)); });
        return std::any_of(supported_extensions.begin(), supported_extensions.end(), [extension_lower](const std::string& extension_iter) { return (extension_iter.compare(extension_lower) == 0); });
    }
}

struct ExplorerCommandHandler : public winrt::implements<ExplorerCommandHandler, IExplorerCommand> {
public:
    // IExplorerCommand implementation:

    IFACEMETHODIMP GetTitle(_In_opt_ IShellItemArray* items, _Outptr_ PWSTR* name) {
        // Provide name for display in File Explorer context menu entry
        UNREFERENCED_PARAMETER(items);
        return SHStrDupW(menu_entry_title, name);
    }

    IFACEMETHODIMP GetIcon(_In_opt_ IShellItemArray* items, _Outptr_ PWSTR* icon) {
        // Provide icon for display in File Explorer context menu entry
        // Get path to application exe and use it as source for icon
        UNREFERENCED_PARAMETER(items);
        std::filesystem::path module_path{ wil::GetModuleFileNameW<std::wstring>(wil::GetModuleInstanceHandle()) };
        module_path = module_path.remove_filename();
        module_path /= exe_filename;
        return SHStrDupW(module_path.c_str(), icon);
    }

    IFACEMETHODIMP GetToolTip(_In_opt_ IShellItemArray* items, _Outptr_ PWSTR* infoTip) {
        UNREFERENCED_PARAMETER(items);
        *infoTip = nullptr;
        return E_NOTIMPL;
    }

    IFACEMETHODIMP GetCanonicalName(_Out_ GUID* guidCommandName) {
        *guidCommandName = GUID_NULL;
        return S_OK;
    }

    IFACEMETHODIMP GetState(_In_opt_ IShellItemArray* items, _In_ BOOL okToBeSlow, _Out_ EXPCMDSTATE* cmdState) {
        // Provide state of File Explorer context menu entry
        // Hide it if registry setting indicates that it should be disabled, else it is enabled
        UNREFERENCED_PARAMETER(okToBeSlow);

        // Check if it is a folder or supported file extension
        bool is_folder{ false };
        bool is_supported_extension{ false };
        if (items) {
            DWORD count;
            RETURN_IF_FAILED(items->GetCount(&count));
            if (count > 0) {
                winrt::com_ptr<IShellItem> item;
                if (SUCCEEDED(items->GetItemAt(0, item.put()))) {
                    SFGAOF attribute{};
                    if (SUCCEEDED(item->GetAttributes(SFGAO_FOLDER | SFGAO_STREAM, &attribute))) {
                        if ((attribute & SFGAO_FOLDER) && !(attribute & SFGAO_STREAM))
                            is_folder = true;
                        else {
                            wil::unique_cotaskmem_string path;
                            if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) {
                                std::filesystem::path filepath{ path.get() };
                                is_supported_extension = IsSupportedFileExtension(filepath.extension().string());
                            }
                        }
                    }
                }
            }
        }

        // Check for files
        if (!is_supported_extension && !is_folder) {
            *cmdState = ECS_HIDDEN;
            return S_OK;
        }
        try {
#ifdef MEDIAINFO_QT
            if (!RegGetBool(HKEY_CURRENT_USER, L"Software\\MediaArea.net\\MediaInfo", L"shellExtension"))
#else
            if (!RegGetDword(HKEY_CURRENT_USER, L"Software\\MediaArea\\MediaInfo", L"ShellExtension") && !is_folder)
#endif // MEDIAINFO_QT
            {
                *cmdState = ECS_HIDDEN;
                return S_OK;
            }
        }
        catch (...) {
            // Error reading reg, default enabled
        }

#ifndef MEDIAINFO_QT
        // Check for folders
        try {
            if (!RegGetDword(HKEY_CURRENT_USER, L"Software\\MediaArea\\MediaInfo", L"ShellExtension_Folder") && is_folder) {
                *cmdState = ECS_HIDDEN;
                return S_OK;
            }
        }
        catch (...) {
            // Error reading reg, default enabled
        }
#endif // MEDIAINFO_QT

        // Default enabled if not disabled
        *cmdState = ECS_ENABLED;
        return S_OK;
    }

    IFACEMETHODIMP GetFlags(_Out_ EXPCMDFLAGS* flags) {
        *flags = ECF_DEFAULT;
        return S_OK;
    }

    IFACEMETHODIMP EnumSubCommands(_Outptr_ IEnumExplorerCommand** enumCommands) {
        *enumCommands = nullptr;
        return E_NOTIMPL;
    }

    IFACEMETHODIMP Invoke(_In_opt_ IShellItemArray* items, _In_opt_ IBindCtx* bindCtx) {
        // Process items passed by File Explorer when context menu entry is invoked
        UNREFERENCED_PARAMETER(bindCtx);

        // Return if no items
        if (!items)
            return S_OK;

        // Get count
        DWORD count;
        RETURN_IF_FAILED(items->GetCount(&count));

        // Get path to application exe
        std::filesystem::path module_path{ wil::GetModuleFileNameW<std::wstring>(wil::GetModuleInstanceHandle()) };
        module_path = module_path.remove_filename();
        module_path /= exe_filename;

        // Prepare cmd line string to invoke application ("path\to\application.exe" "path\to\firstitem" "path\to\nextitem" ...)
        auto command = wil::str_printf<std::wstring>(LR"-("%s")-", module_path.c_str()); // Path to application.exe
        // Add multiple selected files/folders to cmd line as parameters
        for (DWORD i = 0; i < count; ++i) {
            winrt::com_ptr<IShellItem> item;
            auto result = items->GetItemAt(i, item.put());
            if (SUCCEEDED(result)) {
                wil::unique_cotaskmem_string path;
                result = item->GetDisplayName(SIGDN_FILESYSPATH, &path);
                if (SUCCEEDED(result)) {
                    // Append the item path to the existing command, adding quotes and escapes as needed
                    command = wil::str_printf<std::wstring>(LR"-(%s %s)-", command.c_str(), QuoteForCommandLineArg(path.get()).c_str());
                }
            }
        }

        // Invoke application using CreateProcess with the command string prepared above
        wil::unique_process_information process_info;
        STARTUPINFOW startup_info = { sizeof(startup_info) };
        RETURN_IF_WIN32_BOOL_FALSE(CreateProcessW(
            nullptr,
            command.data(),
            nullptr /* lpProcessAttributes */,
            nullptr /* lpThreadAttributes */,
            false /* bInheritHandles */,
            CREATE_NO_WINDOW,
            nullptr,
            nullptr,
            &startup_info,
            &process_info));

        return S_OK;
    }
};

struct DECLSPEC_UUID(SHELLEXT_GUID) ClassFactory : public winrt::implements<ClassFactory, IClassFactory> {
public:

    IFACEMETHODIMP CreateInstance(_In_opt_ IUnknown * pUnkOuter, _In_ REFIID riid, _COM_Outptr_ void** ppvObject) noexcept override {
        UNREFERENCED_PARAMETER(pUnkOuter);
        try {
            return winrt::make<ExplorerCommandHandler>()->QueryInterface(riid, ppvObject);
        }
        catch (...) {
            return winrt::to_hresult();
        }
    }

    IFACEMETHODIMP LockServer(_In_ BOOL fLock) noexcept override {
        if (fLock)
            ++winrt::get_module_lock();
        else
            --winrt::get_module_lock();
        return S_OK;
    }
};

_Check_return_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv) {
    if (ppv == nullptr)
        return E_POINTER;
    *ppv = nullptr;
    if (riid != IID_IClassFactory && riid != IID_IUnknown)
        return E_NOINTERFACE;
    if (rclsid != __uuidof(ClassFactory))
        return E_INVALIDARG;
    try {
        return winrt::make<ClassFactory>()->QueryInterface(riid, ppv);
    }
    catch (...) {
        return winrt::to_hresult();
    }
}

__control_entrypoint(DllExport)
STDAPI DllCanUnloadNow(void) {
    if (winrt::get_module_lock())
        return S_FALSE;
    winrt::clear_factory_cache();
    return S_OK;
}