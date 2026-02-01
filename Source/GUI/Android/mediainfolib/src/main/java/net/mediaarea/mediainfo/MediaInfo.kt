/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

class MediaInfo {
    companion object {
        // load the native library.
        init {
            System.loadLibrary("zen") // load ZenLib manually (fix Android < 4.3)
            System.loadLibrary("mediainfo")
        }
    }

    enum class Stream {
        General,
        Video,
        Audio,
        Text,
        Other,
        Image,
        Menu,
        Max
    }

    enum class Info {
        Name,
        Text,
        Measure,
        Options,
        Name_Text,
        Measure_Text,
        Info,
        HowTo,
        Domain,
        Max
    }

    val mi: Long = Init()

    external fun Init(): Long
    external fun Destroy(): Int
    private external fun OpenFd(fd: Int, name: String): Int
    private external fun Open(name:String ): Int
    fun Open(fd: Int, name: String): Int {
        return OpenFd(fd, name)
    }
    external fun Open_Buffer_Init(fileSize: Long, fileOffset: Long): Int
    external fun Open_Buffer_Continue(buffer: ByteArray, bufferSize: Long): Int
    external fun Open_Buffer_Continue_GoTo_Get(): Long
    external fun Open_Buffer_Finalize(): Long
    external fun Close(): Int
    external fun Inform(): String
    private external fun GetI(streamKind: Int, streamNumber: Int, parameter: Int, infoKind: Int): String
    private external fun GetS(streamKind: Int, streamNumber: Int, parameter: String, infoKind: Int, searchKind: Int): String
    fun Get(streamKind: Stream, streamNumber: Int, parameter: Int, infoKind: Info = Info.Text) : String {
        return GetI(streamKind.ordinal, streamNumber, parameter, infoKind.ordinal)
    }
    fun Get(streamKind: Stream, streamNumber: Int, parameter: String, infoKind: Info = Info.Text, searchKind: Info = Info.Name) : String {
        return GetS(streamKind.ordinal, streamNumber, parameter, infoKind.ordinal, searchKind.ordinal)
    }
    external fun Option(option: String, value: String = ""): String
    external fun State_Get(): Int
    private external fun Count_Get(streamKind: Int, streamNumber: Int): Int
    fun Count_Get(streamKind: Stream, streamNumber: Int = -1): Int {
        return Count_Get(streamKind.ordinal, streamNumber)
    }
}
