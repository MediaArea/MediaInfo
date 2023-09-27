/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import androidx.room.Entity
import androidx.room.PrimaryKey
import androidx.room.ColumnInfo

@Entity(tableName = "reports")
data class Report(@PrimaryKey(autoGenerate = true)
                   @ColumnInfo(name = "id")
                   val id: Int,
                   @ColumnInfo(name = "filename")
                   val filename: String,
                   @ColumnInfo(name = "report", typeAffinity = ColumnInfo.BLOB)
                   val report: ByteArray,
                   @ColumnInfo(name = "version")
                   val version: String) {
    override fun toString(): String {
        return filename
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as Report

        if (id != other.id) return false
        if (filename != other.filename) return false
        if (!report.contentEquals(other.report)) return false
        if (version != other.version) return false

        return true
    }

    override fun hashCode(): Int {
        var result = id
        result = 31 * result + filename.hashCode()
        result = 31 * result + report.contentHashCode()
        result = 31 * result + version.hashCode()
        return result
    }
}