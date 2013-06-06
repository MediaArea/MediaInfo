

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Fri Oct 12 18:30:59 2012
 */
/* Compiler settings for MediaInfoShellExt.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __MediaInfoShellExt_h__
#define __MediaInfoShellExt_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */

#ifndef __IMediaInfoShellExt__FWD_DEFINED__
#define __IMediaInfoShellExt__FWD_DEFINED__
typedef interface IMediaInfoShellExt_ IMediaInfoShellExt_;

#endif  /* __IMediaInfoShellExt__FWD_DEFINED__ */


#ifndef __MediaInfoShellExt__FWD_DEFINED__
#define __MediaInfoShellExt__FWD_DEFINED__

#ifdef __cplusplus
typedef class MediaInfoShellExt_ MediaInfoShellExt_;
#else
typedef struct MediaInfoShellExt_ MediaInfoShellExt_;
#endif /* __cplusplus */

#endif  /* __MediaInfoShellExt__FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif


#ifndef __IMediaInfoShellExt__INTERFACE_DEFINED__
#define __IMediaInfoShellExt__INTERFACE_DEFINED__

/* interface IMediaInfoShellExt_ */
/* [unique][helpstring][uuid][object] */


EXTERN_C const IID IID_IMediaInfoShellExt_;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6482FE39-D67B-49E5-BAA1-EBDDB318055D")
    IMediaInfoShellExt_ : public IUnknown
    {
    public:
    };


#else   /* C style interface */

    typedef struct IMediaInfoShellExt_Vtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
            IMediaInfoShellExt_ * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */
            _COM_Outptr_  void **ppvObject);

        ULONG ( STDMETHODCALLTYPE *AddRef )(
            IMediaInfoShellExt_ * This);

        ULONG ( STDMETHODCALLTYPE *Release )(
            IMediaInfoShellExt_ * This);

        END_INTERFACE
    } IMediaInfoShellExt_Vtbl;

    interface IMediaInfoShellExt_
    {
        CONST_VTBL struct IMediaInfoShellExt_Vtbl *lpVtbl;
    };



#ifdef COBJMACROS


#define IMediaInfoShellExt__QueryInterface(This,riid,ppvObject) \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )

#define IMediaInfoShellExt__AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) )

#define IMediaInfoShellExt__Release(This)   \
    ( (This)->lpVtbl -> Release(This) )


#endif /* COBJMACROS */


#endif  /* C style interface */




#endif  /* __IMediaInfoShellExt__INTERFACE_DEFINED__ */



#ifndef __MediaInfoShellExtLib_LIBRARY_DEFINED__
#define __MediaInfoShellExtLib_LIBRARY_DEFINED__

/* library MediaInfoShellExtLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_MediaInfoShellExtLib;

EXTERN_C const CLSID CLSID_MediaInfoShellExt_;

#ifdef __cplusplus

class DECLSPEC_UUID("869C14C8-1830-491F-B575-5F9AB40D2B42")
MediaInfoShellExt_;
#endif
#endif /* __MediaInfoShellExtLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


