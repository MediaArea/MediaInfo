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
import kotlinx.coroutines.CoroutineDispatcher
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.File

class ReportViewModel(private val dataSource: ReportDao) : ViewModel() {

    private val _loadingCount = MutableStateFlow(0)
    val isLoading: StateFlow<Boolean> = _loadingCount
        .map { count -> count > 0 }
        .stateIn(
            scope = viewModelScope,
            started = SharingStarted.WhileSubscribed((5000)),
            initialValue = false
        )

    private val _navigateToReport = MutableSharedFlow<Int>()
    val navigateToReport = _navigateToReport.asSharedFlow()

    fun addFiles(
        contentResolver: ContentResolver,
        uris: List<Uri>,
        isMultiple: Boolean = false,
        dispatcher: CoroutineDispatcher = Dispatchers.IO
    ) {
        viewModelScope.launch {
            _loadingCount.update { it + 1 }

            try {
                withContext(dispatcher) {
                    var lastInsertedId: Int? = null

                    for (uri in uris) {
                        var fd: ParcelFileDescriptor? = null
                        var displayName = ""

                        when (uri.scheme) {
                            "content" -> {
                                try {
                                    val cursor: Cursor? =
                                        contentResolver.query(uri, null, null, null, null, null)
                                    cursor?.use {
                                        // moveToFirst() returns false if the cursor has 0 rows
                                        if (cursor.moveToFirst()) {
                                            // DISPLAY_NAME is provider-specific, and might not be the file name
                                            val column =
                                                cursor.getColumnIndexOrThrow(OpenableColumns.DISPLAY_NAME)
                                            if (column >= 0)
                                                displayName = cursor.getString(column)
                                        }
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

                        fd.use {
                            val report: ByteArray = Core.createReport(it.detachFd(), displayName)
                            lastInsertedId =
                                insertReport(Report(0, displayName, report, Core.version)).toInt()
                        }
                    }

                    // Don't go to report view when opening multiple files
                    if (uris.size == 1 && !isMultiple && lastInsertedId != null) {
                        _navigateToReport.emit(lastInsertedId)
                    }
                }
            } finally {
                _loadingCount.update { it - 1 }
            }
        }
    }

    @Suppress("unused")
    suspend fun getLastId(): Int? = dataSource.getLastId()

    suspend fun getReport(id: Int): Report? = dataSource.getReport(id)

    fun getAllReports(): Flow<List<Report>> = dataSource.getAllReports()

    suspend fun insertReport(report: Report): Long = dataSource.insertReport(report)

    @Suppress("unused")
    fun updateReport(report: Report) {
        viewModelScope.launch(Dispatchers.IO) {
            dataSource.updateReport(report)
        }
    }

    @Suppress("unused")
    fun deleteReport(report: Report) {
        viewModelScope.launch(Dispatchers.IO) {
            dataSource.deleteReport(report)
        }
    }

    fun deleteReport(id: Int) {
        viewModelScope.launch(Dispatchers.IO) {
            dataSource.deleteReport(id)
        }
    }

    fun deleteAllReports() {
        viewModelScope.launch(Dispatchers.IO) {
            dataSource.deleteAllReports()
        }
    }
}
