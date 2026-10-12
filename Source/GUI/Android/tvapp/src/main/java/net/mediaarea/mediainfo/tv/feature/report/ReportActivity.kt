/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo.tv.feature.report

import android.content.Context
import android.content.ContextWrapper
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.view.KeyEvent
import android.webkit.WebView
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.BackHandler
import androidx.activity.compose.setContent
import androidx.appcompat.app.AppCompatActivity
import androidx.compose.foundation.background
import androidx.compose.foundation.focusable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.focus.FocusRequester
import androidx.compose.ui.focus.focusRequester
import androidx.compose.ui.graphics.RectangleShape
import androidx.compose.ui.input.key.onKeyEvent
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.tv.material3.MaterialTheme
import androidx.tv.material3.Surface
import androidx.tv.material3.Text
import androidx.tv.material3.surfaceColorAtElevation
import net.mediaarea.mediainfo.tv.MediaInfoApplication
import net.mediaarea.mediainfo.tv.R
import net.mediaarea.mediainfo.tv.ui.theme.AndroidTheme

// Extend from AppCompatActivity instead of ComponentActivity for WebView dark mode
class ReportActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContent {
            ReportView(intent)
        }
    }
}

private fun Context.findActivity(): ComponentActivity? = when (this) {
    is ComponentActivity -> this
    is ContextWrapper -> baseContext.findActivity()
    else -> null
}

@Composable
private fun ReportView(intent: Intent) {
    val context = LocalContext.current
    val app = context.applicationContext as MediaInfoApplication
    val viewModel: ReportViewModel = viewModel(factory = app.factory)
    val isDark by viewModel.darkModeEnabled.collectAsStateWithLifecycle()
    val fileName: String? = intent.getStringExtra("FILE_NAME")
    val toastMessage = stringResource(R.string.please_wait)
    val isLoading = viewModel.isLoading
    val report = viewModel.report

    // Prevent leaving activity while loading
    // since cancelling MediaInfo's parsing is not implemented
    BackHandler(enabled = isLoading) {
        Toast.makeText(
            context,
            toastMessage,
            Toast.LENGTH_SHORT
        ).show()
    }

    // Parsing
    LaunchedEffect(Unit) {
        val uri: Uri? = intent.data
        val completeName: String? = intent.getStringExtra("COMPLETE_NAME")
        viewModel.generateReport(uri, completeName)
    }

    // Report display
    AndroidTheme(isInDarkTheme = isDark) {
        Report(isLoading, report, fileName)
    }
}

@Composable
private fun Report(isLoading: Boolean, report: String?, fileName: String?) {
    Surface(
        modifier = Modifier.fillMaxSize(),
        shape = RectangleShape
    ) {
        Column {
            Box(
                modifier = Modifier
                    .background(MaterialTheme.colorScheme.surfaceColorAtElevation(0.5.dp))
                    .fillMaxWidth()
                    .padding(5.dp),
                contentAlignment = Alignment.Center
            ) {
                Text(stringResource(R.string.app_name) + " - " + fileName)
            }
            if (isLoading) {
                Box(
                    modifier = Modifier.fillMaxSize(),
                    contentAlignment = Alignment.Center,
                ) {
                    CircularProgressIndicator()
                }
            } else {
                if (report == null) {
                    Box(
                        modifier = Modifier.fillMaxSize(),
                        contentAlignment = Alignment.Center,
                    ) {
                        Text("Something unexpected has occurred.")
                    }
                } else {
                    WebViewComposable(report)
                }
            }
        }
    }
}

@Composable
private fun WebViewComposable(report: String) {
    val context = LocalContext.current
    val focusRequester = remember { FocusRequester() }
    AndroidView(
        modifier = Modifier
            .fillMaxSize()
            .focusable()
            .focusRequester(focusRequester)
            .onKeyEvent { keyEvent ->
                // Finish the activity immediately on back pressed
                // Without this, it will require two back presses to exit report activity
                // because the first will remove focus from WebView and only the second will go back
                if (keyEvent.nativeKeyEvent.keyCode == KeyEvent.KEYCODE_BACK &&
                    keyEvent.nativeKeyEvent.action == KeyEvent.ACTION_UP
                ) {

                    context.findActivity()?.finish()

                    return@onKeyEvent true
                }
                false // Let other handlers deal with it
            },
        factory = { ctx ->
            WebView(ctx).apply {
                // use loadDataWithBaseURL instead of loadData to prevent issues from special characters
                loadDataWithBaseURL(null, report, "text/html", "utf-8", null)
            }
        }
    )
    // Get focus on WebView immediately on launch
    // Without this, it will require an enter press before
    // the WebView gains focus and can be scrolled with arrow keys
    LaunchedEffect(Unit) {
        focusRequester.requestFocus()
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun ReportPreviewLoading() {
    AndroidTheme(isInDarkTheme = true) {
        Report(true, null, "Test.mkv")
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun ReportPreviewLoaded() {
    AndroidTheme(isInDarkTheme = true) {
        Report(false, "<!DOCTYPE html>", "Test.mkv")
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun ReportPreviewUnexpected() {
    AndroidTheme(isInDarkTheme = true) {
        Report(false, null, "Test.mkv")
    }
}
