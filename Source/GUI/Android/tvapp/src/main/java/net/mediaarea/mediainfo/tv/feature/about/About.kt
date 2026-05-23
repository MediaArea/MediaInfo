/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo.tv.feature.about

import androidx.compose.foundation.Image
import androidx.compose.foundation.focusable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.RectangleShape
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.tv.material3.MaterialTheme
import androidx.tv.material3.Surface
import androidx.tv.material3.Text
import net.mediaarea.mediainfo.tv.BuildConfig
import net.mediaarea.mediainfo.tv.Core
import net.mediaarea.mediainfo.tv.R
import net.mediaarea.mediainfo.tv.ui.theme.AndroidTheme

@Composable
fun About() {
    AboutContent(mediainfoLibVersion = Core.version)
}

@Composable
private fun AboutContent(
    mediainfoLibVersion: String
) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(80.dp)
            .focusable(),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        val modifier = Modifier.padding(10.dp)
        Image(
            painter = painterResource(id = R.mipmap.ic_launcher_foreground),
            contentDescription = stringResource(R.string.app_name) + " icon",
        )
        Text(
            text = stringResource(R.string.app_name),
            style = MaterialTheme.typography.headlineMedium,
            modifier = modifier,
        )
        Text(
            text = stringResource(R.string.about_about_text)
                .replace("%MI_VERSION%", BuildConfig.VERSION_NAME)
                .replace("%MIL_VERSION%", mediainfoLibVersion),
            style = MaterialTheme.typography.bodyLarge,
            textAlign = TextAlign.Center,
            modifier = modifier,
        )
        Text(
            text = stringResource(R.string.about_description_text),
            style = MaterialTheme.typography.bodyMedium,
            textAlign = TextAlign.Center,
            modifier = modifier,
        )
        Text(
            text = stringResource(R.string.about_copyright_text),
            style = MaterialTheme.typography.bodyMedium,
            textAlign = TextAlign.Center,
            modifier = modifier,
        )
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun AboutPreview() {
    AndroidTheme {
        Surface(
            modifier = Modifier.fillMaxSize(),
            shape = RectangleShape
        ) {
            AboutContent(BuildConfig.VERSION_NAME)
        }
    }
}
