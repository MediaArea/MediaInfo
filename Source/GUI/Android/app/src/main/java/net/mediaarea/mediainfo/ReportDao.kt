/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import androidx.room.Dao
import androidx.room.Delete
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query
import androidx.room.Update
import kotlinx.coroutines.flow.Flow

@Dao
interface ReportDao {

    @Query("SELECT MAX(id) FROM reports")
    suspend fun getLastId(): Int?

    @Query("SELECT * FROM reports WHERE id = :id")
    suspend fun getReport(id: Int): Report?

    @Query("SELECT * FROM reports ORDER BY id")
    fun getAllReports(): Flow<List<Report>>

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertReport(report: Report): Long

    @Update(onConflict = OnConflictStrategy.REPLACE)
    suspend fun updateReport(report: Report)

    @Delete
    suspend fun deleteReport(report: Report)

    @Query("DELETE FROM reports WHERE id = :id")
    suspend fun deleteReport(id: Int)

    @Query("DELETE FROM reports")
    suspend fun deleteAllReports()
}
