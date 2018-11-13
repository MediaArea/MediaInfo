/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.content.Context

object Injection {
    private fun provideReportDataSource(context: Context): ReportDao {
        val database: ReportsDatabase = ReportsDatabase.getInstance(context)
        return database.reportDao()
    }

    fun provideViewModelFactory(context: Context): ViewModelFactory {
        val dataSource: ReportDao = provideReportDataSource(context)
        return ViewModelFactory(dataSource)
    }
}