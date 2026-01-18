/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.content.ContentResolver
import android.database.Cursor
import android.net.Uri
import android.os.ParcelFileDescriptor
import android.provider.OpenableColumns
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import io.reactivex.Completable
import io.reactivex.Flowable
import io.reactivex.Single
import io.reactivex.schedulers.Schedulers
import kotlinx.coroutines.CoroutineDispatcher
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch
import kotlinx.coroutines.rx2.await
import kotlinx.coroutines.withContext
import java.io.File

class ReportViewModel(private val dataSource: ReportDao) : ViewModel() {

    private val _isLoading = MutableStateFlow(false)
    val isLoading = _isLoading.asStateFlow()

    private val _navigateToReport = MutableSharedFlow<Int>()
    val navigateToReport = _navigateToReport.asSharedFlow()

    fun addFiles(
        contentResolver: ContentResolver,
        uris: List<Uri>,
        isMultiple: Boolean = false,
        dispatcher: CoroutineDispatcher = Dispatchers.IO
    ) {
        viewModelScope.launch {
            _isLoading.value = true

            withContext(dispatcher) {
                for (uri in uris) {
                    var fd: ParcelFileDescriptor? = null
                    var displayName = ""

                    when (uri.scheme) {
                        "content" -> {
                            try {
                                val cursor: Cursor? =
                                    contentResolver.query(uri, null, null, null, null, null)
                                // moveToFirst() returns false if the cursor has 0 rows
                                if (cursor != null && cursor.moveToFirst()) {
                                    // DISPLAY_NAME is provider-specific, and might not be the file name
                                    val column =
                                        cursor.getColumnIndexOrThrow(OpenableColumns.DISPLAY_NAME)
                                    if (column >= 0) {
                                        displayName = cursor.getString(column)
                                    }
                                    cursor.close()
                                }
                            } catch (_: Exception) {
                            }
                            try {
                                fd = contentResolver.openFileDescriptor(uri, "r")
                            } catch (_: Exception) {
                            }
                        }

                        "file" -> {
                            val file = File(uri.path.orEmpty())
                            try {
                                fd = ParcelFileDescriptor.open(
                                    file,
                                    ParcelFileDescriptor.MODE_READ_ONLY
                                )
                            } catch (_: Exception) {
                            }

                            displayName = uri.lastPathSegment.orEmpty()
                        }
                    }

                    if (fd == null) {
                        continue
                    }

                    val report: ByteArray = Core.createReport(fd.detachFd(), displayName)

                    insertReport(Report(0, displayName, report, Core.version))
                        .subscribeOn(Schedulers.io()).await()
                }

                // Don't go to report view when opening multiple files
                if (uris.size == 1 && !isMultiple) {
                    val lastId = getLastId().subscribeOn(Schedulers.io()).await()
                    _navigateToReport.emit(lastId)
                }
            }

            _isLoading.value = false
        }
    }

    fun getLastId(): Single<Int> {
        return dataSource.getLastId()
    }

    fun getReport(id: Int): Single<Report> {
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
