/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.arch.persistence.room.RoomDatabase
import android.arch.persistence.room.Database
import android.arch.persistence.room.Room
import android.content.Context

@Database(entities = [Report::class], version = 1, exportSchema = false)
abstract class ReportsDatabase : RoomDatabase() {
    companion object {
        @Volatile private var INSTANCE: ReportsDatabase? = null

        fun getInstance(context: Context): ReportsDatabase =  INSTANCE?: synchronized(this) {
            INSTANCE?: buildDatabase(context).also {
                INSTANCE = it
            }
        }

        private fun buildDatabase(context: Context) = Room.databaseBuilder(context.applicationContext,
                ReportsDatabase::class.java, "Reports.db").build()
    }

    abstract fun reportDao(): ReportDao
}