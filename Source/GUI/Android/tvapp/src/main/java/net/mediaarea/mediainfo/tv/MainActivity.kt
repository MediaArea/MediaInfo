/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo.tv

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.annotation.StringRes
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.selection.selectableGroup
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Folder
import androidx.compose.material.icons.filled.Info
import androidx.compose.material.icons.filled.Settings
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.RectangleShape
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.navigation.NavGraph.Companion.findStartDestination
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.currentBackStackEntryAsState
import androidx.navigation.compose.rememberNavController
import androidx.tv.material3.DrawerState
import androidx.tv.material3.DrawerValue
import androidx.tv.material3.Icon
import androidx.tv.material3.MaterialTheme
import androidx.tv.material3.NavigationDrawer
import androidx.tv.material3.NavigationDrawerItem
import androidx.tv.material3.Surface
import androidx.tv.material3.Text
import androidx.tv.material3.rememberDrawerState
import androidx.tv.material3.surfaceColorAtElevation
import net.mediaarea.mediainfo.tv.feature.about.About
import net.mediaarea.mediainfo.tv.feature.browse.Browse
import net.mediaarea.mediainfo.tv.feature.settings.Settings
import net.mediaarea.mediainfo.tv.ui.theme.AndroidTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            MediaInfoAppContainer()
        }
    }
}

@Composable
private fun MediaInfoAppContainer() {
    val app = LocalContext.current.applicationContext as MediaInfoApplication
    val viewModel: MainViewModel = viewModel(factory = app.factory)

    // Collect the state from the ViewModel in a lifecycle-aware way
    val isDark by viewModel.darkModeEnabled.collectAsStateWithLifecycle()

    AndroidTheme(isInDarkTheme = isDark) {
        MediaInfoApp()
    }
}

@Composable
private fun MediaInfoApp(drawerState: DrawerState = rememberDrawerState(initialValue = DrawerValue.Open)) {
    Surface(
        modifier = Modifier.fillMaxSize(),
        shape = RectangleShape
    ) {
        val navController = rememberNavController()
        val navBackStackEntry by navController.currentBackStackEntryAsState()
        val currentRoute = navBackStackEntry?.destination?.route
        val items =
            listOf(
                Screen.Browse,
                Screen.Settings,
                Screen.About
            )

        NavigationDrawer(
            drawerState = drawerState,
            drawerContent = { drawerValue ->
                Column(
                    modifier = Modifier
                        .background(MaterialTheme.colorScheme.surfaceColorAtElevation(0.dp))
                        .fillMaxHeight()
                        .padding(12.dp)
                        .selectableGroup(),
                    horizontalAlignment = Alignment.Start,
                    verticalArrangement = Arrangement.spacedBy(10.dp, Alignment.CenterVertically)
                ) {
                    items.forEach { screen ->
                        NavigationDrawerItem(
                            selected = currentRoute == screen.route,
                            onClick = {
                                navController.navigate(screen.route) {
                                    // Pop up to start destination to avoid building up a large stack
                                    popUpTo(navController.graph.findStartDestination().id) {
                                        saveState = true
                                    }
                                    launchSingleTop = true
                                    restoreState = true
                                }
                            },
                            leadingContent = {
                                Icon(imageVector = screen.icon, contentDescription = null)
                            }
                        ) {
                            Text(stringResource(screen.resourceId))
                        }
                    }
                }
                if (drawerValue == DrawerValue.Open) {
                    MediaInfoHeader(Modifier.align(Alignment.TopStart))
                }
            }
        ) {
            NavHost(navController, startDestination = Screen.Browse.route) {
                composable(Screen.Browse.route) { Browse() }
                composable(Screen.Settings.route) { Settings() }
                composable(Screen.About.route) { About() }
            }
        }
    }
}

private sealed class Screen(
    val route: String,
    @get:StringRes val resourceId: Int,
    val icon: ImageVector
) {
    object Browse : Screen("browse", R.string.browse, Icons.Default.Folder)
    object Settings : Screen("settings", R.string.settings, Icons.Default.Settings)
    object About : Screen("about", R.string.about, Icons.Default.Info)
}

@Composable
private fun MediaInfoHeader(modifier: Modifier = Modifier) {
    Box(
        modifier = modifier.padding(20.dp)
    ) {
        Text(
            text = stringResource(R.string.app_name),
            style = MaterialTheme.typography.headlineSmall,
        )
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun MediaInfoAppPreview() {
    AndroidTheme {
        MediaInfoApp()
    }
}

@Preview(showBackground = true, device = "id:tv_4k")
@Composable
private fun MediaInfoAppPreviewDrawerClosed() {
    AndroidTheme {
        MediaInfoApp(rememberDrawerState(initialValue = DrawerValue.Closed))
    }
}
