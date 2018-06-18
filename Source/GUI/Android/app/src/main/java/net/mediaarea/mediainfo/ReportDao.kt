/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.arch.persistence.room.Dao
import android.arch.persistence.room.Query
import android.arch.persistence.room.Insert
import android.arch.persistence.room.Update
import android.arch.persistence.room.Delete
import android.arch.persistence.room.OnConflictStrategy

import io.reactivex.Flowable
import io.reactivex.Single

@Dao
interface ReportDao {
    @Query("SELECT MAX(id) FROM reports")
    fun getLastId(): Single<Int>

    @Query("SELECT * FROM reports WHERE id = :id")
    fun getReport(id: Int): Flowable<Report>

    @Query("SELECT * FROM reports")
    fun getAllReports(): Flowable<List<Report>>

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    fun insertReport(report: Report)

    @Update(onConflict = OnConflictStrategy.REPLACE)
    fun updateReport(report: Report)

    @Delete()
    fun deleteReport(report: Report)

    @Query("DELETE FROM reports WHERE id = :id")
    fun deleteReport(id: Int)

    @Query("DELETE FROM reports")
    fun deleteAllReports()
}