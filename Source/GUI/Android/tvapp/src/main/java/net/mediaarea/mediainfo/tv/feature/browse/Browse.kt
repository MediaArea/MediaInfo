/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo.tv.feature.browse

import android.Manifest
import android.content.Intent
import android.os.Build
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Image
import androidx.compose.material.icons.filled.Movie
import androidx.compose.material.icons.filled.MusicNote
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.RectangleShape
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.core.net.toUri
import androidx.lifecycle.ProcessLifecycleOwner
import androidx.lifecycle.compose.LifecycleResumeEffect
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.tv.material3.Button
import androidx.tv.material3.MaterialTheme
import androidx.tv.material3.Surface
import androidx.tv.material3.Text
import com.google.accompanist.permissions.ExperimentalPermissionsApi
import com.google.accompanist.permissions.rememberMultiplePermissionsState
import net.mediaarea.mediainfo.tv.MediaInfoApplication
import net.mediaarea.mediainfo.tv.R
import net.mediaarea.mediainfo.tv.feature.report.ReportActivity
import net.mediaarea.mediainfo.tv.ui.components.MediaListItem
import net.mediaarea.mediainfo.tv.ui.theme.AndroidTheme

@OptIn(ExperimentalPermissionsApi::class)
@Composable
fun Browse() {
    val permissionsState = rememberMultiplePermissionsState(
        permissions = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            listOf(
                Manifest.permission.READ_MEDIA_IMAGES,
                Manifest.permission.READ_MEDIA_VIDEO,
                Manifest.permission.READ_MEDIA_AUDIO
            )
        } else {
            listOf(Manifest.permission.READ_EXTERNAL_STORAGE)
        }
    )

    if (permissionsState.allPermissionsGranted) {
        MediaBrowserScreen()
    } else {
        PermissionDeniedNotice { permissionsState.launchMultiplePermissionRequest() }
    }
}

@Composable
private fun MediaBrowserScreen() {
    val app = LocalContext.current.applicationContext as MediaInfoApplication
    val viewModel: MediaViewModel = viewModel(factory = app.factory)
    val state by viewModel.uiState.collectAsState()

    // Fetch MediaStore at least once or on app process resume
    LifecycleResumeEffect(
        key1 = Unit,
        lifecycleOwner = ProcessLifecycleOwner.get()
    ) {
        viewModel.loadMedia()
        onPauseOrDispose {}
    }

    MediaBrowserScreenContent(state)
}

@Composable
private fun MediaBrowserScreenContent(state: MediaUiState) {
    val context = LocalContext.current

    Box(modifier = Modifier.fillMaxSize(), contentAlignment = Alignment.Center) {
        when (state) {
            is MediaUiState.Loading -> {
                Column(
                    horizontalAlignment = Alignment.CenterHorizontally,
                    verticalArrangement = Arrangement.spacedBy(12.dp)
                ) {
                    CircularProgressIndicator()
                    Text(stringResource(R.string.loading))
                }
            }

            is MediaUiState.Empty -> {
                Text(stringResource(R.string.empty))
            }

            is MediaUiState.Error -> {
                Text(stringResource(R.string.error) + ": " + state.message)
            }

            is MediaUiState.Success -> {
                LazyColumn(
                    modifier = Modifier.fillMaxSize(),
                    contentPadding = PaddingValues(32.dp),
                    verticalArrangement = Arrangement.spacedBy(12.dp)
                ) {
                    items(state.items) { item ->
                        MediaListItem(
                            name = item.name,
                            icon = when (item.type) {
                                MediaType.Video -> Icons.Default.Movie
                                MediaType.Audio -> Icons.Default.MusicNote
                                MediaType.Image -> Icons.Default.Image
                            },
                            path = item.path,
                            date = item.dateModified,
                            onClick = {
                                val intent = Intent(context, ReportActivity::class.java).apply {
                                    data = item.uri
                                    putExtra("COMPLETE_NAME", item.path)
                                    putExtra("FILE_NAME", item.name)
                                }
                                context.startActivity(intent)
                            }
                        )
                    }
                }
            }
        }
    }
}

@Composable
private fun PermissionDeniedNotice(onRequestPermission: () -> Unit) {
    Column(
        modifier = Modifier.fillMaxSize(),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(
            text = stringResource(R.string.permission_header),
            style = MaterialTheme.typography.headlineMedium,
            color = Color.White
        )
        Spacer(modifier = Modifier.height(16.dp))
        Text(
            text = stringResource(R.string.permission_text, stringResource(R.string.app_name)),
            style = MaterialTheme.typography.bodyLarge,
            textAlign = TextAlign.Center,
            modifier = Modifier.padding(horizontal = 48.dp)
        )
        Spacer(modifier = Modifier.height(32.dp))
        Button(onClick = onRequestPermission) {
            Text(stringResource(R.string.permission_button))
        }
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun PermissionDeniedNoticePreview() {
    AndroidTheme {
        Surface(
            modifier = Modifier.fillMaxSize(),
            shape = RectangleShape
        ) {
            PermissionDeniedNotice {}
        }
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun MediaBrowserScreenLoadingPreview() {
    AndroidTheme {
        Surface(
            modifier = Modifier.fillMaxSize(),
            shape = RectangleShape
        ) {
            MediaBrowserScreenContent(MediaUiState.Loading)
        }
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun MediaBrowserScreenSuccessPreview() {
    val testMediaList = listOf(
        MediaItem(
            id = 1L,
            uri = "content://media/external/images/media/1".toUri(),
            name = "Test image.jpg",
            type = MediaType.Image,
            path = "/storage/emulated/0/Pictures/Test image.jpg",
            dateModified = 1767877545000L
        ),
        MediaItem(
            id = 2L,
            uri = "content://media/external/video/media/2".toUri(),
            name = "Test video.mkv",
            type = MediaType.Video,
            path = "/storage/emulated/0/Movies/Test video.mkv",
            dateModified = 1767865330000L
        ),
        MediaItem(
            id = 3L,
            uri = "content://media/external/audio/media/3".toUri(),
            name = "Test audio.flac",
            type = MediaType.Audio,
            path = "/storage/emulated/0/Music/Recordings/Test audio.flac",
            dateModified = 1767856522000L
        )
    )
    AndroidTheme {
        Surface(
            modifier = Modifier.fillMaxSize(),
            shape = RectangleShape
        ) {
            MediaBrowserScreenContent(MediaUiState.Success(testMediaList))
        }
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun MediaBrowserScreenEmptyPreview() {
    AndroidTheme {
        Surface(
            modifier = Modifier.fillMaxSize(),
            shape = RectangleShape
        ) {
            MediaBrowserScreenContent(MediaUiState.Empty)
        }
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun MediaBrowserScreenErrorPreview() {
    AndroidTheme {
        Surface(
            modifier = Modifier.fillMaxSize(),
            shape = RectangleShape
        ) {
            MediaBrowserScreenContent(MediaUiState.Error("Test error"))
        }
    }
}
