/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import androidx.annotation.NonNull
import androidx.room.Entity
import androidx.room.PrimaryKey
import androidx.room.ColumnInfo

@Entity(tableName = "reports")
data class Report(@PrimaryKey(autoGenerate = true)
                   @ColumnInfo(name = "id")
                   @NonNull
                   val id: Int,
                   @ColumnInfo(name = "filename")
                   @NonNull
                   val filename: String,
                   @ColumnInfo(name = "report", typeAffinity = ColumnInfo.BLOB)
                   @NonNull
                   val report: ByteArray,
                   @ColumnInfo(name = "version")
                   @NonNull
                   val version: String) {
    override fun toString(): String {
        return filename
    }
}