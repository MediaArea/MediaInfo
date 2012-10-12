// MediaInfoShellExt_.h : Declaration of the CMediaInfoShellExt_

#pragma once
#include "resource.h"       // main symbols
#include "MediaInfoShellExt.h"
#include <comdef.h>
struct __declspec(uuid("00021500-0000-0000-c000-000000000046")) IQueryInfo;
#include <shlobj.h>
#include <string>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CMediaInfoShellExt_

class ATL_NO_VTABLE CMediaInfoShellExt_ :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CMediaInfoShellExt_, &CLSID_MediaInfoShellExt_>,
    public IPersistFile,
    public IQueryInfo
{
public:
    CMediaInfoShellExt_()
    {
    }

DECLARE_REGISTRY_RESOURCEID(IDR_MEDIAINFOSHELLEXT_)

DECLARE_NOT_AGGREGATABLE(CMediaInfoShellExt_)

BEGIN_COM_MAP(CMediaInfoShellExt_)
    COM_INTERFACE_ENTRY(IPersistFile)
    COM_INTERFACE_ENTRY(IQueryInfo)
END_COM_MAP()



    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

protected:
    std::wstring FileName;

public:
    // IPersistFile
    STDMETHOD(GetClassID)(LPCLSID)      { return E_NOTIMPL; }
    STDMETHOD(IsDirty)()                { return E_NOTIMPL; }
    STDMETHOD(Load)(LPCOLESTR, DWORD);
    STDMETHOD(Save)(LPCOLESTR, BOOL)    { return E_NOTIMPL; }
    STDMETHOD(SaveCompleted)(LPCOLESTR) { return E_NOTIMPL; }
    STDMETHOD(GetCurFile)(LPOLESTR*)    { return E_NOTIMPL; }

    // IQueryInfo
    STDMETHOD(GetInfoFlags)(DWORD*)     { return E_NOTIMPL; }
    STDMETHOD(GetInfoTip)(DWORD, LPWSTR*);
};

OBJECT_ENTRY_AUTO(__uuidof(MediaInfoShellExt_), CMediaInfoShellExt_)
