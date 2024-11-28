/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

// Functions for registration and removal of MediaInfo sparse package

#include "pch.h"
#include "Installer.h"

using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::Management::Deployment;

using namespace MediaInfo_SparsePackage::Installer;

const std::wstring SparsePackageName = L"MediaInfo";
const std::wstring SparsePackageFileName = L"MediaInfo_SparsePackage.msix";

static const std::wstring GetInstallationPath() {
    std::filesystem::path module_path{ wil::GetModuleFileNameW<std::wstring>(wil::GetModuleInstanceHandle()) };
    return module_path.remove_filename().wstring();
}

static Package GetSparsePackage()
{
    PackageManager packageManager;
    IIterable<Package> packages;

    try {
        packages = packageManager.FindPackages();
    }
    catch (winrt::hresult_error) {
        return NULL;
    }

    for (const Package& package : packages) {
        if (package.Id().Name() != SparsePackageName)
            continue;

        return package;
    }

    return NULL;
}

static HRESULT RegisterSparsePackage()
{
    // Cannot handle sparse packages in safe-mode
    if (GetSystemMetrics(SM_CLEANBOOT) > 0)
        return S_FALSE;

    PackageManager packageManager;
    StagePackageOptions options;

    const std::wstring externalLocation = GetInstallationPath();
    const std::wstring sparsePkgPath = externalLocation + L"\\" + SparsePackageFileName;

    Uri externalUri(externalLocation);
    Uri packageUri(sparsePkgPath);

    options.ExternalLocationUri(externalUri);

    auto deploymentOperation = packageManager.StagePackageByUriAsync(packageUri, options);
    auto deployResult = deploymentOperation.get();

    if (FAILED(deployResult.ExtendedErrorCode()))
        return deployResult.ExtendedErrorCode();

    Package package = GetSparsePackage();
    if (package == NULL)
        return S_FALSE;

    winrt::hstring familyName = package.Id().FamilyName();
    deploymentOperation = packageManager.ProvisionPackageForAllUsersAsync(familyName);
    deployResult = deploymentOperation.get();

    if (FAILED(deployResult.ExtendedErrorCode()))
        return deployResult.ExtendedErrorCode();

    return S_OK;
}


static HRESULT UnregisterSparsePackage()
{
    // Cannot handle sparse packages in safe-mode
    if (GetSystemMetrics(SM_CLEANBOOT) > 0)
        return S_FALSE;

    const Package package = GetSparsePackage();
    if (package == NULL)
        return S_FALSE;

    PackageManager packageManager;
    winrt::hstring fullName = package.Id().FullName();
    auto deploymentOperation = packageManager.RemovePackageAsync(fullName, RemovalOptions::RemoveForAllUsers);
    auto deployResult = deploymentOperation.get();

    if (FAILED(deployResult.ExtendedErrorCode()))
        return deployResult.ExtendedErrorCode();

    return S_OK;
}

STDAPI MediaInfo_SparsePackage::Installer::Install()
{
    HRESULT result;

    // remove any old existing versions first
    result = UnregisterSparsePackage();
    if (FAILED(result))
        return result;

    result = RegisterSparsePackage();
    if (FAILED(result))
        return result;

    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    return S_OK;
}

STDAPI MediaInfo_SparsePackage::Installer::Uninstall()
{
    HRESULT result = UnregisterSparsePackage();
    if (FAILED(result))
        return result;

    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    return S_OK;
}