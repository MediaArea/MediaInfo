/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo.tv

import net.mediaarea.mediainfo.MediaInfo
import net.mediaarea.mediainfo.tv.feature.settings.DisplayCaptions

object Core {
    data class ReportView(
        val name: String,
        val desc: String,
        val mime: String,
        val exportable: Boolean
    ) {
        override fun toString(): String {
            return desc
        }
    }

    val mi: MediaInfo = MediaInfo()
    val views: MutableList<ReportView> = mutableListOf()
    val version: String = mi.Option("Info_Version").replace("MediaInfoLib - v", "")

    init {
        // populate views list
        val viewsCsv: String = mi.Option("Info_OutputFormats_CSV")
        viewsCsv.split("\n").forEach {
            val view: List<String> = it.split(",")
            if (view.size > 2)
                views.add(ReportView(view[0], view[1], view[2], true))
        }
    }

    fun generateReport(
        fd: Int,
        name: String,
        legacyStreamDisplay: Boolean,
        displayCaptions: DisplayCaptions
    ): String {
        mi.Option("Inform", "HTML")
        mi.Option("LegacyStreamDisplay", if (legacyStreamDisplay) "1" else "0")
        mi.Option("File_DisplayCaptions", displayCaptions.key)

        mi.Open(fd, name)
        val report = mi.Inform()
        mi.Close()

        return report
    }
}
