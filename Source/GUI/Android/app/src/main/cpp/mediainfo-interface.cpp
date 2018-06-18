/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include <jni.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <ZenLib/Ztring.h>
#include <MediaInfo/MediaInfo_Internal.h>

#include <android/log.h>

#define BUFFER_SIZE 1024 * 1024

using namespace MediaInfoLib;

// Ensure jlong can hold a pointer
static_assert(sizeof(jlong) >= sizeof(nullptr), "JNI jlong can't hold a pointer");

/**
 * Helpers
 */

//------------------------------------------------------------------------------
Ztring Jstring2Ztring(JNIEnv* _env, jstring jstr)
{
    const char* str = _env->GetStringUTFChars(jstr, JNI_FALSE);

    Ztring toReturn = Ztring().From_UTF8(str);

    _env->ReleaseStringUTFChars(jstr, str);

    return toReturn;
}

//------------------------------------------------------------------------------
MediaInfo_Internal* GetMiObj(JNIEnv* _env, jobject _this)
{
    jclass cls = _env->GetObjectClass(_this);
    if (cls == nullptr)
        return nullptr;

    jfieldID miId = _env->GetFieldID(cls, "mi", "J");
    if (miId == nullptr)
        return nullptr;

    MediaInfo_Internal* mi = (MediaInfo_Internal*)_env->GetLongField(_this, miId);
    if (mi == nullptr)
        return nullptr;

    return mi;
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jlong JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Init(JNIEnv* _env, jobject _this)
{
    return (jlong)new MediaInfo_Internal();
}


//------------------------------------------------------------------------------
extern "C" JNIEXPORT jint JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Destroy(JNIEnv* _env, jobject _this)
{
    delete GetMiObj(_env, _this);

    return 0;
}

/**
 * Interface
 */

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jint JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Open(JNIEnv* _env, jobject _this, jint fd, jstring name)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return 1;

    int64u size = (int64u)-1;

    struct stat s;
    if (fstat(fd, &s) != -1 && s.st_size > 0)
        size = (int64u)s.st_size;

    mi->Option(__T("File_FileName"), Jstring2Ztring(_env, name));

    int8u* buffer = new int8u[BUFFER_SIZE];

    mi->Open_Buffer_Init(size, 0);
    while (1) {

        ssize_t count = read(fd, (void*)buffer, BUFFER_SIZE);

        if (count < 0)
            break; // error

        bitset<32> state = mi->Open_Buffer_Continue((int8u*)buffer, count);

        // bit 3 set means finalized
        if (state.test(3))
            break;

        // test if there is a MediaInfo request to go elsewhere
        int64u seekTo = mi->Open_Buffer_Continue_GoTo_Get();
        if (seekTo != (int64u)-1)
        {
            off_t offset = lseek(fd, (off_t)seekTo, SEEK_SET);
            if (offset >= 0) {
                mi->Open_Buffer_Init(size, (int64u)offset); // inform MediaInfo we have seek
                continue;
            }
        }

        // EOF and no seekTo request
        if (count == 0)
            break;
    }
    mi->Open_Buffer_Finalize();

    delete[] buffer;
    close(fd);

    return 0;
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jint JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Open_1Buffer_1Init(JNIEnv* _env, jobject _this, jlong fileSize, jlong  fileOffset)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return (jint)-1;

    return mi->Open_Buffer_Init((int64u)fileSize, (int64u) fileOffset);
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jint JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Open_1Buffer_1Continue(JNIEnv* _env, jobject _this, jbyteArray buffer, jlong  bufferSize)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return (jint)-1;

    int8u*  buff = (int8u*)_env->GetByteArrayElements(buffer, JNI_FALSE);
    jint toReturn = (jint)mi->Open_Buffer_Continue(buff, (size_t)bufferSize).to_ulong();

    _env->ReleaseByteArrayElements(buffer, (jbyte*)buff, 0);

    return toReturn;
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jlong JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Open_1Buffer_1Continue_1GoTo_1Get(JNIEnv* _env, jobject _this)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return (jint)-1;

    return (jlong)mi->Open_Buffer_Continue_GoTo_Get();
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jlong JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Open_1Buffer_1Finalize(JNIEnv* _env, jobject _this)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return (jlong)-1;

    return (jlong)mi->Open_Buffer_Finalize();
}


//------------------------------------------------------------------------------
extern "C" JNIEXPORT jint JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Close(JNIEnv* _env, jobject _this)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return 1;

    mi->Close();

    return 0;
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jstring JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Inform(JNIEnv* _env, jobject _this)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return _env->NewStringUTF("");

    return _env->NewStringUTF(Ztring(MediaInfo_Internal::Inform(mi)).To_UTF8().c_str());
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jstring JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_GetI(JNIEnv* _env, jobject _this, jint streamKind, jint streamNumber, jint parameter, jint infoKind)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return _env->NewStringUTF("");

    return _env->NewStringUTF(Ztring(mi->Get((stream_t) streamKind, (size_t)streamNumber, (size_t)parameter, (info_t)infoKind)).To_UTF8().c_str());
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jstring JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_GetS(JNIEnv* _env, jobject _this, jint streamKind, jint streamNumber, jstring parameter, jint infoKind, jint searchKind)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return _env->NewStringUTF("");

    return _env->NewStringUTF(Ztring(mi->Get((stream_t) streamKind, (size_t)streamNumber, Jstring2Ztring(_env, parameter), (info_t)infoKind, (info_t)searchKind)).To_UTF8().c_str());
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jstring JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Option(JNIEnv* _env, jobject _this, jstring option, jstring value)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return _env->NewStringUTF("");

    return _env->NewStringUTF(Ztring(mi->Option(Jstring2Ztring(_env, option), Jstring2Ztring(_env, value))).To_UTF8().c_str());
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jint JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_State_1Get(JNIEnv* _env, jobject _this)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return 0;

    return (jint)mi->State_Get();
}

//------------------------------------------------------------------------------
extern "C" JNIEXPORT jint JNICALL
Java_net_mediaarea_mediainfo_MediaInfo_Count_1Get(JNIEnv* _env, jobject _this, jint streamKind, jint streamNumber)
{
    MediaInfo_Internal* mi = GetMiObj(_env, _this);
    if (mi == nullptr)
        return 0;

    return (jint)mi->Count_Get((stream_t)streamKind, streamNumber==-1?(size_t)-1:(size_t)streamNumber);
}