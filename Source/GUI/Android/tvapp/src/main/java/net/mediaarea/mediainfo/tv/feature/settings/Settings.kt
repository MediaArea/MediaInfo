/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo.tv.feature.settings

import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.RectangleShape
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.tv.material3.MaterialTheme
import androidx.tv.material3.Surface
import androidx.tv.material3.Text
import net.mediaarea.mediainfo.tv.MediaInfoApplication
import net.mediaarea.mediainfo.tv.R
import net.mediaarea.mediainfo.tv.ui.components.CategoryHeader
import net.mediaarea.mediainfo.tv.ui.components.MultiOptionListItem
import net.mediaarea.mediainfo.tv.ui.components.ToggleListItem
import net.mediaarea.mediainfo.tv.ui.theme.AndroidTheme

@Composable
fun Settings() {
    val app = LocalContext.current.applicationContext as MediaInfoApplication
    val viewModel: SettingsViewModel = viewModel(factory = app.factory)
    val state by viewModel.uiState.collectAsStateWithLifecycle()
    SettingsContent(
        state = state,
        onEvent = { viewModel.onEvent(it) }
    )
}

@Composable
private fun SettingsContent(
    state: MediaInfoSettings,
    onEvent: (SettingsEvent) -> Unit
) {
    LazyColumn(contentPadding = PaddingValues(32.dp)) {
        item {
            Text(
                modifier = Modifier.padding(bottom = 15.dp),
                text = stringResource(R.string.settings),
                style = MaterialTheme.typography.headlineMedium,
            )
        }
        item {
            CategoryHeader(stringResource(R.string.header_appearance))
        }
        item {
            ToggleListItem(
                title = stringResource(R.string.dark_mode),
                description = if (state.darkModeEnabled)
                    stringResource(R.string.enabled)
                else
                    stringResource(R.string.disabled),
                isChecked = state.darkModeEnabled,
                onToggle = { onEvent(SettingsEvent.ToggleDarkMode(it)) }
            )
        }
        item {
            val optionsLabels = SortOrder.entries.toTypedArray().map { stringResource(it.labelRes) }
            val selectedIndex = SortOrder.entries.toTypedArray().indexOf(state.sortOrder)
            MultiOptionListItem(
                label = stringResource(R.string.file_sort_order),
                supportingText = stringResource(R.string.file_sort_order_desc),
                options = optionsLabels,
                selectedIndex = selectedIndex,
                onOptionChanged = { onEvent(SettingsEvent.CycleSortOrder) }
            )
        }
        item {
            CategoryHeader(stringResource(R.string.header_mediainfolib))
        }
        item {
            ToggleListItem(
                title = stringResource(R.string.legacy_stream_display),
                description = stringResource(R.string.legacy_stream_display_desc),
                isChecked = state.legacyStreamDisplayEnabled,
                onToggle = { onEvent(SettingsEvent.ToggleLegacyStreamDisplay(it)) }
            )
        }
        item {
            val options = DisplayCaptions.entries.toTypedArray().map { stringResource(it.labelRes) }
            val selectedIdx = DisplayCaptions.entries.toTypedArray().indexOf(state.displayCaptions)
            MultiOptionListItem(
                label = stringResource(R.string.display_captions),
                supportingText = stringResource(R.string.display_captions_desc),
                options = options,
                selectedIndex = selectedIdx,
                onOptionChanged = { onEvent(SettingsEvent.CycleDisplayCaptions) }
            )
        }
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun SettingsContentPreviewDark() {
    val isDark = true
    AndroidTheme(isDark) {
        Surface(
            modifier = Modifier.fillMaxSize(),
            shape = RectangleShape
        ) {
            SettingsContent(
                state = MediaInfoSettings(
                    darkModeEnabled = isDark,
                    sortOrder = SortOrder.DATE,
                    legacyStreamDisplayEnabled = false,
                    displayCaptions = DisplayCaptions.COMMAND
                ),
                onEvent = {} // Empty lambda for preview
            )
        }
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun SettingsContentPreviewLight() {
    val isDark = false
    AndroidTheme(isDark) {
        Surface(
            modifier = Modifier.fillMaxSize(),
            shape = RectangleShape
        ) {
            SettingsContent(
                state = MediaInfoSettings(
                    darkModeEnabled = isDark,
                    sortOrder = SortOrder.DATE,
                    legacyStreamDisplayEnabled = true,
                    displayCaptions = DisplayCaptions.COMMAND
                ),
                onEvent = {} // Empty lambda for preview
            )
        }
    }
}
