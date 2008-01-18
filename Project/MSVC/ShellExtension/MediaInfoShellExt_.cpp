// MediaInfoShellExt_.cpp : Implementation of CMediaInfoShellExt_

#include "stdafx.h"
#include "MediaInfoShellExt_.h"
#include <MediaInfoDLL/MediaInfoDLL_Static.h>


// CMediaInfoShellExt_

HRESULT CMediaInfoShellExt_::Load (LPCOLESTR wszFilename, DWORD)
{
    //Save FileName
    FileName = wszFilename;

    //OK
	return S_OK;
}

HRESULT CMediaInfoShellExt_::GetInfoTip (DWORD, LPWSTR* ppwszTip)
{
    LPMALLOC        Malloc;
    std::wstring    ToolTip;

    //Get an IMalloc interface from the shell.
    if (FAILED(SHGetMalloc(&Malloc)))
        return E_FAIL;

    //Creating tooltip
	ToolTip=_T("MediaInfo:\n");
	MediaInfoLib::MediaInfo I;
	I.Open(FileName);
	ToolTip+=I.Inform();

    //Allocate a buffer for Explorer.  Note that the must pass the string 
    //back as a Unicode string, so the string length is multiplied by the 
    //size of a Unicode character.
    *ppwszTip=(LPWSTR)Malloc->Alloc((ToolTip.size()+1)*sizeof(wchar_t));

    //Release the IMalloc interface now that we're done using it.
    Malloc->Release();

    //Enough memory?
	if (*ppwszTip==NULL)
        return E_OUTOFMEMORY;

    //Use the Unicode string copy function to put the tooltip text in the buffer.
    wcscpy_s(*ppwszTip, ToolTip.size()+1, T2COLE(ToolTip.c_str()));

    //OK
	return S_OK;
}