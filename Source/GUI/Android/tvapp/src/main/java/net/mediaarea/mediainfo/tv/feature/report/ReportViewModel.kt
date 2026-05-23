/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo.tv.feature.report

import android.content.ContentResolver
import android.net.Uri
import android.os.ParcelFileDescriptor
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.CoroutineDispatcher
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.launch
import net.mediaarea.mediainfo.tv.Core
import net.mediaarea.mediainfo.tv.feature.settings.FLOW_TIMEOUT
import net.mediaarea.mediainfo.tv.feature.settings.SettingsRepository

class ReportViewModel(
    private val contentResolver: ContentResolver,
    private val repository: SettingsRepository
) : ViewModel() {

    private var wasTriggered = false
    var isLoading by mutableStateOf(true)
    var report: String? by mutableStateOf(null)

    val darkModeEnabled: StateFlow<Boolean> = repository.settingsFlow
        .map { it.darkModeEnabled }
        .stateIn(
            scope = viewModelScope,
            started = SharingStarted.WhileSubscribed(FLOW_TIMEOUT),
            initialValue = true
        )

    fun generateReport(
        uri: Uri?,
        completeName: String?,
        dispatcher: CoroutineDispatcher = Dispatchers.IO
    ) {
        // Prevent parallel/repeated execution
        if (wasTriggered) return

        wasTriggered = true

        viewModelScope.launch(dispatcher) {
            val settingsFlowFirst = repository.settingsFlow.first()
            val legacyStreamDisplayEnabled = settingsFlowFirst.legacyStreamDisplayEnabled
            val displayCaptions = settingsFlowFirst.displayCaptions
            var fd: ParcelFileDescriptor? = null
            if (uri != null) {
                try {
                    fd = contentResolver.openFileDescriptor(uri, "r")
                } catch (_: Exception) {
                }
            }
            if (fd != null && completeName != null) {
                report =
                    Core.generateReport(
                        fd.detachFd(),
                        completeName,
                        legacyStreamDisplayEnabled,
                        displayCaptions
                    )
            }
            isLoading = false
        }
    }
}
