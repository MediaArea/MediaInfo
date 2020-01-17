/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import androidx.lifecycle.ViewModel
import io.reactivex.Completable
import io.reactivex.Flowable
import io.reactivex.Single

class ReportViewModel(private val dataSource: ReportDao) : ViewModel() {
    fun getLastId(): Single<Int> {
        return dataSource.getLastId()
    }

    fun getReport(id: Int): Flowable<Report> {
        return dataSource.getReport(id)
    }

    fun getAllReports(): Flowable<List<Report>> {
        return dataSource.getAllReports()
    }

    fun insertReport(report: Report): Completable {
        return Completable.fromAction {
            dataSource.insertReport(report)
        }
    }

    /* unused
    fun updateReport(report: Report): Completable {
        return Completable.fromAction {
            dataSource.updateReport(report)
        }
    }
    */

    /* unused
    fun deleteReport(report: Report): Completable {
        return Completable.fromAction {
            dataSource.deleteReport(report)
        }
    }
    */

    fun deleteReport(id: Int): Completable {
        return Completable.fromAction {
            dataSource.deleteReport(id)
        }
    }

    fun deleteAllReports(): Completable {
        return Completable.fromAction {
            dataSource.deleteAllReports()
        }
    }
}