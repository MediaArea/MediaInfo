/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo.tv.ui.components

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.MusicNote
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.tv.material3.Icon
import androidx.tv.material3.ListItem
import androidx.tv.material3.MaterialTheme
import androidx.tv.material3.Surface
import androidx.tv.material3.Text
import androidx.tv.material3.darkColorScheme
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

/**
 * A TV ListItem for displaying media items.
 *
 * @param name The headlineContent of the item such as the file name.
 * @param icon An ImageVector to be used as the leadingContent.
 * @param path The path of the item shown as supportingContent.
 * @param date The date of the item as the number of milliseconds since January 1, 1970, 00:00:00 GMT.
 * @param onClick Function to be executed when the item is clicked.
 */
@Composable
fun MediaListItem(
    name: String,
    icon: ImageVector,
    path: String,
    date: Long,
    onClick: () -> Unit
) {
    // ListItem automatically animates scale/color on focus
    ListItem(
        selected = false,
        onClick = onClick,
        headlineContent = {
            Text(text = name, maxLines = 1, overflow = TextOverflow.Ellipsis)
        },
        supportingContent = {
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Text(
                    text = path,
                    modifier = Modifier.weight(1f),
                    maxLines = 1,
                    overflow = TextOverflow.Ellipsis
                )
                Text(
                    text = SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault())
                        .format(Date(date)),
                    modifier = Modifier.padding(start = 5.dp),
                    maxLines = 1
                )
            }
        },
        leadingContent = {
            Icon(imageVector = icon, contentDescription = null)
        }
    )
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun MediaListItemPreview() {
    MaterialTheme(colorScheme = darkColorScheme()) {
        Surface {
            LazyColumn(
                contentPadding = PaddingValues(32.dp),
                verticalArrangement = Arrangement.spacedBy(12.dp)
            ) {
                item {
                    MediaListItem(
                        name = "Test audio.flac",
                        icon = Icons.Default.MusicNote,
                        path = "/storage/emulated/0/Music/Recordings/Test audio.flac",
                        date = 1767856522000L,
                        onClick = {}
                    )
                }
            }
        }
    }
}
