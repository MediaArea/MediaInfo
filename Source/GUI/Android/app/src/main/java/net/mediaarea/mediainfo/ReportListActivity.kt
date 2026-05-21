/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import kotlin.jvm.*
import kotlinx.coroutines.launch

import androidx.activity.result.contract.ActivityResultContracts
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.app.AppCompatDelegate
import androidx.core.app.ActivityCompat
import androidx.core.content.edit
import androidx.core.net.toUri
import androidx.core.view.updateLayoutParams
import androidx.core.view.updatePadding
import androidx.core.view.ViewCompat
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.lifecycleScope
import androidx.lifecycle.repeatOnLifecycle
import androidx.lifecycle.ViewModelProvider
import androidx.preference.PreferenceManager.getDefaultSharedPreferences
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import com.google.android.material.dialog.MaterialAlertDialogBuilder

import android.os.Build
import android.os.Bundle
import android.net.Uri
import android.content.Intent
import android.widget.FrameLayout
import android.widget.TextView
import android.widget.Toast
import android.content.SharedPreferences
import android.content.pm.PackageManager
import android.content.res.Configuration
import android.content.res.Resources
import android.provider.Settings
import android.view.*

import com.yariksoffice.lingver.Lingver
import kotlinx.coroutines.flow.collectLatest
import java.io.BufferedReader
import java.util.*

import  net.mediaarea.mediainfo.databinding.ActivityReportListBinding
import  net.mediaarea.mediainfo.databinding.ClearButtonBinding
import  net.mediaarea.mediainfo.databinding.ReportListContentBinding
import  net.mediaarea.mediainfo.databinding.HelloLayoutBinding

/**
 * An activity representing a list of Pings. This activity
 * has different presentations for handset and tablet-size devices. On
 * handsets, the activity presents a list of items, which when touched,
 * lead to a [ReportDetailActivity] representing
 * item details. On tablets, the activity presents the list of items and
 * item details side-by-side using two vertical panes.
 */
class ReportListActivity : AppCompatActivity(), ReportActivityListener {
    private lateinit var activityReportListBinding: ActivityReportListBinding
    private lateinit var helloLayoutBinding: HelloLayoutBinding
    private lateinit var subscriptionManager: SubscriptionManager
    private lateinit var reportModel: ReportViewModel
    private var twoPane: Boolean = false
    private var reports: List<Report> = listOf()
    private val pendingFileUris: MutableList<Uri> = mutableListOf()
    private val pendingIntents: MutableList<Intent> = mutableListOf()

    private val openFile = registerForActivityResult(ActivityResultContracts.GetMultipleContents()) { uris ->
        if (uris.isNotEmpty())
            reportModel.addFiles(contentResolver, uris)
    }

    private val startSubscribeActivityForResult = registerForActivityResult(ActivityResultContracts.StartActivityForResult()) { result ->
        if (result.resultCode == RESULT_OK)
            Toast.makeText(applicationContext, R.string.thanks_text, Toast.LENGTH_SHORT).show()
    }

    private val startShowReportForResult = registerForActivityResult(ActivityResultContracts.StartActivityForResult()) { result ->
        if (result.resultCode == RESULT_OK) {
            result.data?.let {
                val id = it.getIntExtra(Core.ARG_REPORT_ID, -1)
                if (id != -1 && twoPane)
                    showReport(id)
            }
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        // If user wants to enable ACCESS_MEDIA_LOCATION permission, but the prompt has been blocked
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q
            && permissions[0] == android.Manifest.permission.ACCESS_MEDIA_LOCATION
            && grantResults[0] == PackageManager.PERMISSION_DENIED
            && !shouldShowRequestPermissionRationale(permissions[0])) {
            showGeolocationHelpDialog()
            pendingFileUris.clear()
            return
        }
        when (requestCode) {
            READ_EXTERNAL_STORAGE_PERMISSION_REQUEST -> {
                if ((grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED)) {
                    reportModel.addFiles(contentResolver, pendingFileUris.toList()) // Create a copy of pendingFileUris to avoid getting a cleared item in reportModel.addFiles@viewModelScope subroutine
                    pendingFileUris.clear()
                } else {
                    // Abort all pending request
                    pendingFileUris.clear()
                }
            }
            ACCESS_MEDIA_LOCATION_PERMISSION_REQUEST_OPENFILE -> {
                openFile.launch("*/*") // Lets user open files anyway, even if the location data may be redacted out
            }
            ACCESS_MEDIA_LOCATION_PERMISSION_REQUEST_SENDFILE -> {
                reportModel.addFiles(contentResolver, pendingFileUris.toList()) // Create a copy of pendingFileUris to avoid getting a cleared item in reportModel.addFiles@viewModelScope subroutine
                pendingFileUris.clear()
            }
        }
    }

    private fun openPermissionsSettings() {
        val intent = Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS)
        intent.addCategory(Intent.CATEGORY_DEFAULT)
        intent.data = "package:${this.packageName}".toUri()
        startActivity(intent)
    }

    private fun showGeolocationHelpDialog() {
        MaterialAlertDialogBuilder(this)
            .setTitle(R.string.permissions_geolocation_title)
            .setMessage(R.string.permissions_geolocation_help)
            .setPositiveButton(android.R.string.ok) { _, _ ->
                openPermissionsSettings()
            }
            .setNegativeButton(android.R.string.cancel, null)
            .show()
    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
    }

    override fun onNewIntent(intent: Intent) {
        super.onNewIntent(intent)
        setIntent(intent)

        pendingIntents.add(intent)
    }

    override fun onResume() {
        super.onResume()

        for (intent in pendingIntents) {
            handleIntent(intent)
        }
        pendingIntents.clear()
    }

    private fun handleUri(uri: Uri, isMultiple: Boolean = false) {
        when (uri.scheme) {
            "file" -> {
                if (Build.VERSION.SDK_INT >= 23) {
                    if (checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                        pendingFileUris.add(uri)
                        ActivityCompat.requestPermissions(this@ReportListActivity,
                            arrayOf(android.Manifest.permission.READ_EXTERNAL_STORAGE),
                            READ_EXTERNAL_STORAGE_PERMISSION_REQUEST)
                        return
                    }
                }
            }
            "content" -> {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
                    if (checkSelfPermission(android.Manifest.permission.ACCESS_MEDIA_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                        pendingFileUris.add(uri)
                        requestMediaLocationPermission(ACCESS_MEDIA_LOCATION_PERMISSION_REQUEST_SENDFILE)
                        return
                    }
                }
            }
        }
        reportModel.addFiles(contentResolver, listOf(uri), isMultiple)
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    private fun requestMediaLocationPermission(requestCode: Int) {
        MaterialAlertDialogBuilder(this)
            .setTitle(R.string.permissions_geolocation_title)
            .setMessage(R.string.permissions_geolocation_summary)
            .setPositiveButton(android.R.string.ok) { _, _ ->
                ActivityCompat.requestPermissions(
                    this@ReportListActivity,
                    arrayOf(android.Manifest.permission.ACCESS_MEDIA_LOCATION),
                    requestCode
                )
            }
            .setNegativeButton(android.R.string.cancel) { _, _ ->
                when (requestCode) {
                    ACCESS_MEDIA_LOCATION_PERMISSION_REQUEST_OPENFILE -> {
                        openFile.launch("*/*") // Lets user open files anyway, even if the location data may be redacted out
                    }
                    ACCESS_MEDIA_LOCATION_PERMISSION_REQUEST_SENDFILE -> {
                        reportModel.addFiles(contentResolver, pendingFileUris.toList()) // Create a copy of pendingFileUris to avoid getting a cleared item in reportModel.addFiles@viewModelScope subroutine
                        pendingFileUris.clear()
                    }
                }
            }
            .show()
    }

    private fun handleIntent(intent: Intent) {
        if (intent.action != null) {
            if (intent.getBooleanExtra(OPEN_INTENT_PROCESSED, false)) {
                return
            }

            when (intent.action) {
                Intent.ACTION_VIEW -> {
                    val uri: Uri? = intent.data
                    if (uri != null) {
                        handleUri(uri)
                        intent.putExtra(OPEN_INTENT_PROCESSED, true)
                    }
                }
                Intent.ACTION_SEND -> {
                    val uri =
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                            intent.getParcelableExtra(Intent.EXTRA_STREAM, Uri::class.java)
                        } else {
                            @Suppress("DEPRECATION")
                            intent.getParcelableExtra(Intent.EXTRA_STREAM)
                        }
                    if (uri != null) {
                        handleUri(uri)
                        intent.putExtra(OPEN_INTENT_PROCESSED, true)
                    }
                }
                Intent.ACTION_SEND_MULTIPLE -> {
                    val uriList =
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                            intent.getParcelableArrayListExtra(Intent.EXTRA_STREAM, Uri::class.java)
                        } else {
                            @Suppress("DEPRECATION")
                            intent.getParcelableArrayListExtra(Intent.EXTRA_STREAM)
                        }
                    if (uriList != null) {
                        for (i in uriList.filterNotNull()) {
                            handleUri(i, true)
                        }
                        intent.putExtra(OPEN_INTENT_PROCESSED, true)
                    }
                }
            }
        }
    }

    private fun updatePreferences() {
        val sharedPreferences = getDefaultSharedPreferences(this)
        val key = getString(R.string.preferences_uimode_key)

        when (sharedPreferences?.contains(key)) {
            false, null -> {
                val oldSharedPreferences = getSharedPreferences(getString(R.string.preferences_key), MODE_PRIVATE)
                if (oldSharedPreferences?.contains(key) == true) {
                    oldSharedPreferences.getString(key, "").let {
                        when (it) {
                            "ON" -> {
                                oldSharedPreferences.edit {
                                    remove(key)
                                    putString(key, "on")
                                }
                            }
                            "OFF" -> {
                                oldSharedPreferences.edit {
                                    remove(key)
                                    putString(key, "off")
                                }
                            }
                            else -> {
                            }
                        }
                    }
                }
            }
            true -> {
                try {
                    sharedPreferences.getBoolean(key, false).let {
                        when (it) {
                            false -> {
                                sharedPreferences.edit {
                                    remove(key)
                                    putString(key, "off")
                                }
                            }
                            true -> {
                                sharedPreferences.edit {
                                    remove(key)
                                    putString(key, "on")
                                }
                            }
                        }
                    }
                }
                catch(_: ClassCastException) {}
            }
        }
    }

    private fun applyUiMode() {
        val sharedPreferences = getDefaultSharedPreferences(this)
        val key = getString(R.string.preferences_uimode_key)

        sharedPreferences?.getString(key, "system").let {
            when (it) {
                "off" -> {
                    if (AppCompatDelegate.getDefaultNightMode()!=AppCompatDelegate.MODE_NIGHT_NO) {
                        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO)
                        recreate()
                    }
                }
                "on" -> {
                    if (AppCompatDelegate.getDefaultNightMode()!=AppCompatDelegate.MODE_NIGHT_YES) {
                        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES)
                        recreate()
                    }
                }
                "system" -> {
                    if (AppCompatDelegate.getDefaultNightMode()!=AppCompatDelegate.MODE_NIGHT_FOLLOW_SYSTEM) {
                        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_FOLLOW_SYSTEM)
                        recreate()
                    }
                }
            }
        }
    }

    private fun setLocale() {
        try {
            val stream = applicationContext.resources.openRawResource(R.raw.lang)
            val content = stream.bufferedReader().use(BufferedReader::readText)
            Core.setLocale(content)
        }
        catch (_: Exception) {
        }
    }

    private fun setPrefLocale() {
        val sharedPreferences: SharedPreferences? = getDefaultSharedPreferences(this)
        sharedPreferences?.getBoolean(getString(R.string.preferences_report_translate_key), false).let {
            when (it) {
                false, null -> {
                    Core.setLocale("")
                }
                true -> {
                    setLocale()
                }
            }
        }

        sharedPreferences?.getString(getString(R.string.preferences_locale_key), "system").let {
            val locale: Locale =
                if (it == null || it == "system") {
                    if (Build.VERSION.SDK_INT >= 24) {
                        Resources.getSystem().configuration.locales.get(0)
                    } else {
                        @Suppress("DEPRECATION")
                        Resources.getSystem().configuration.locale
                    }
                } else {
                    val language = it.split("-r")
                    if (language.size > 1) {
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.BAKLAVA) {
                            Locale.of(language[0], language[1])
                        } else {
                            @Suppress("DEPRECATION")
                            Locale(language[0], language[1])
                        }
                    } else {
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.BAKLAVA) {
                            Locale.of(language[0])
                        } else {
                            @Suppress("DEPRECATION")
                            Locale(language[0])
                        }
                    }
                }

            Locale.setDefault(locale)

            if (Lingver.getInstance().getLocale() != locale) {
                Lingver.getInstance().setLocale(this, locale)
                recreate()
            }
        }
    }

    fun showReport(id: Int) {
        intent.putExtra(Core.ARG_REPORT_ID, id)
        if (twoPane) {
            val fragment: ReportDetailFragment = ReportDetailFragment().apply {
                arguments = Bundle().apply {
                    putInt(Core.ARG_REPORT_ID, id)
                }
            }

            supportFragmentManager
                    .beginTransaction()
                    .replace(R.id.report_detail_container, fragment)
                    .commit()

            lifecycleScope.launch {
                title = reportModel.getReport(id)?.filename ?: getString(R.string.app_name)
            }
        } else {
            val intent = Intent(this@ReportListActivity, ReportDetailActivity::class.java)
            intent.putExtra(Core.ARG_REPORT_ID, id)

            startShowReportForResult.launch(intent)
        }
        if (twoPane) activityReportListBinding.reportListLayout.reportList.post {
            val itemRecyclerViewAdapter = activityReportListBinding.reportListLayout.reportList.adapter as? ItemRecyclerViewAdapter
            itemRecyclerViewAdapter?.setSelectedId(id)
            val position = reports.indexOfFirst { it.id == id }
            if (position != -1) {
                activityReportListBinding.reportListLayout.reportList.smoothScrollToPosition(position)
            }
        }
    }

    fun deleteReport(id: Int) {
        reportModel.deleteReport(id)

        if (intent.getIntExtra(Core.ARG_REPORT_ID, -1)==id) {
            intent.putExtra(Core.ARG_REPORT_ID, -1)
        }
        if (twoPane) {
            val fragment = supportFragmentManager.findFragmentById(R.id.report_detail_container)
            if (fragment != null && (fragment as ReportDetailFragment).id == id) {
                supportFragmentManager
                        .beginTransaction()
                        .detach(fragment)
                        .commit()

                title = getString(R.string.app_name)
            }
        }
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.menu_main, menu)

        val onSubscriptionManagerReadyChange: (Boolean) -> Unit = {
            if (subscriptionManager.subscribed.value == false) {
                menu?.findItem(R.id.action_subscribe)?.isEnabled = it
            }
        }

        menu?.findItem(R.id.action_subscribe)?.isEnabled=false
        subscriptionManager.ready.value?.let {
            onSubscriptionManagerReadyChange(it)
        }

        subscriptionManager.ready.observe(this) {
            onSubscriptionManagerReadyChange(it)
        }

        val onSubscriptionManagerSubscribedChange: (Boolean) -> Unit = {
            if (it) {
                if (subscriptionManager.isLifetime.value == true) {
                    menu?.findItem(R.id.action_subscribe).let { item ->
                        item?.title = getString(R.string.subscribe_text)
                        item?.isVisible = false
                    }
                } else {
                    menu?.findItem(R.id.action_subscribe).let { item ->
                        item?.isEnabled = true
                        item?.title = getString(R.string.subscribed_text)

                        item?.setOnMenuItemClickListener { _ ->
                            val intent = Intent(Intent.ACTION_VIEW)
                            intent.data = getString(R.string.subscription_manage_url).replace('|', '&').toUri()
                            startActivity(intent)

                            true
                        }
                    }
                }
            } else {
                menu?.findItem(R.id.action_subscribe).let { item ->
                    item?.title = getString(R.string.subscribe_text)

                    item?.setOnMenuItemClickListener { _ ->
                        val intent = Intent(this, SubscribeActivity::class.java)
                        startSubscribeActivityForResult.launch(intent)

                        true
                    }
                }
            }
        }

        subscriptionManager.subscribed.value?.let {
            onSubscriptionManagerSubscribedChange(it)
        }

        subscriptionManager.subscribed.observe(this) {
            onSubscriptionManagerSubscribedChange(it)
        }

        menu?.findItem(R.id.action_about).let {
            it?.setOnMenuItemClickListener {
                val intent = Intent(this,  AboutActivity::class.java)
                startActivity(intent)

                true
            }
        }

        menu?.findItem(R.id.action_settings).let {
            it?.setOnMenuItemClickListener {
                val intent = Intent(this, SettingsActivity::class.java)
                startActivity(intent)

                true
            }
        }

        return super.onCreateOptionsMenu(menu)
    }

    override fun getReportViewModel(): ReportViewModel {
        return reportModel
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        activityReportListBinding = ActivityReportListBinding.inflate(layoutInflater)
        helloLayoutBinding = HelloLayoutBinding.inflate(layoutInflater)

        WindowCompat.enableEdgeToEdge(window)
        ViewCompat.setOnApplyWindowInsetsListener(activityReportListBinding.appBar) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updatePadding(
                left = bars.left,
                top = bars.top,
                right = bars.right,
            )
            insets
        }
        ViewCompat.setOnApplyWindowInsetsListener(activityReportListBinding.frameLayout) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updatePadding(
                left = bars.left,
                right = bars.right,
                bottom = bars.bottom,
            )
            insets
        }
        ViewCompat.setOnApplyWindowInsetsListener(activityReportListBinding.addButton) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updateLayoutParams<ViewGroup.MarginLayoutParams> {
                bottomMargin = bars.bottom + resources.getDimensionPixelSize(R.dimen.button_margin)
            }
            insets
        }

        setContentView(activityReportListBinding.root)

        setSupportActionBar(activityReportListBinding.toolBar)
        activityReportListBinding.toolBar.title = title

        updatePreferences()

        subscriptionManager = SubscriptionManager.getInstance(application)

        setLocale()

        val onSubscriptionManagerSubscribedChange: (Boolean) -> Unit = {
            if (it) {
                applyUiMode()
                setPrefLocale()
            }
        }

        subscriptionManager.subscribed.value?.let {
            onSubscriptionManagerSubscribedChange(it)
        }

        subscriptionManager.subscribed.observe(this) {
            onSubscriptionManagerSubscribedChange(it)
        }

        val viewModelFactory = Injection.provideViewModelFactory(this)
        reportModel = ViewModelProvider(this, viewModelFactory)[ReportViewModel::class.java]

        setupRecyclerView(activityReportListBinding.reportListLayout.reportList)

        lifecycleScope.launch {
            repeatOnLifecycle(Lifecycle.State.STARTED) {
                launch {
                    reportModel.isLoading.collectLatest { isLoading ->
                        val rootLayout: FrameLayout = activityReportListBinding.frameLayout
                        if (isLoading) {
                            activityReportListBinding.addButton.hide()
                            var found = false
                            for (i: Int in rootLayout.childCount downTo 1) {
                                if (rootLayout.getChildAt(i - 1).id == R.id.spinner_layout)
                                    found = true
                            }
                            if (!found)
                                View.inflate(
                                    this@ReportListActivity,
                                    R.layout.spinner_layout,
                                    rootLayout
                                )
                        } else {
                            activityReportListBinding.addButton.show()
                            for (i: Int in rootLayout.childCount downTo 1) {
                                if (rootLayout.getChildAt(i - 1).id == R.id.spinner_layout)
                                    rootLayout.removeViewAt(i - 1)
                            }
                        }
                    }
                }
                launch {
                    reportModel.navigateToReport.collectLatest { reportId ->
                        showReport(reportId)
                    }
                }
                launch {
                    reportModel.getAllReports().collectLatest {
                        reports = it
                        (activityReportListBinding.reportListLayout.reportList.adapter as? ItemRecyclerViewAdapter)?.updateList(reports)

                        val rootLayout: FrameLayout = findViewById(R.id.frame_layout)
                        if (reports.isEmpty()) {
                            var found = false
                            for (i: Int in rootLayout.childCount downTo 1) {
                                if (rootLayout.getChildAt(i - 1).id == R.id.hello_layout)
                                    found = true
                            }

                            if (!found)
                                View.inflate(this@ReportListActivity, R.layout.hello_layout, rootLayout)

                        } else {
                            for (i: Int in rootLayout.childCount downTo 1) {
                                if (rootLayout.getChildAt(i - 1).id == R.id.hello_layout)
                                    rootLayout.removeViewAt(i - 1)
                            }
                            rootLayout.removeView(helloLayoutBinding.root)
                        }
                    }
                }
            }
        }

        activityReportListBinding.addButton.setOnClickListener {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
                if (checkSelfPermission(android.Manifest.permission.ACCESS_MEDIA_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                    requestMediaLocationPermission(ACCESS_MEDIA_LOCATION_PERMISSION_REQUEST_OPENFILE)
                    return@setOnClickListener
                }
            }
            openFile.launch("*/*")
        }

        // The detail container view will be present only in the
        // large-screen layouts (res/values-w900dp).
        // If this view is present, then the
        // activity should be in two-pane mode.
        if (activityReportListBinding.reportListLayout.reportDetailContainer != null)
            twoPane = true

        if (!twoPane) {
            val fragment = supportFragmentManager.findFragmentById(R.id.report_detail_container)
            if (fragment != null) {
                supportFragmentManager
                    .beginTransaction()
                    .detach(fragment)
                    .commit()
            }
        }

        onNewIntent(intent)
    }

    override fun onStart() {
        super.onStart()

        if (twoPane && intent.getIntExtra(Core.ARG_REPORT_ID, -1)!=-1) {
            showReport(intent.getIntExtra(Core.ARG_REPORT_ID, -1))
        }
    }

    private fun setupRecyclerView(recyclerView: RecyclerView) {
        recyclerView.adapter = ItemRecyclerViewAdapter()
    }

    sealed class ReportListItem {
        data class ReportData(val report: Report) : ReportListItem()
        object ClearButton : ReportListItem()
    }
    class ReportDiffCallback : DiffUtil.ItemCallback<ReportListItem>() {
        override fun areItemsTheSame(oldItem: ReportListItem, newItem: ReportListItem): Boolean {
            return if (oldItem is ReportListItem.ReportData && newItem is ReportListItem.ReportData) {
                oldItem.report.id == newItem.report.id
            } else {
                oldItem is ReportListItem.ClearButton && newItem is ReportListItem.ClearButton
            }
        }
        override fun areContentsTheSame(oldItem: ReportListItem, newItem: ReportListItem): Boolean {
            return if (oldItem is ReportListItem.ReportData && newItem is ReportListItem.ReportData) {
                oldItem.report.id == newItem.report.id &&
                oldItem.report.filename == newItem.report.filename &&
                oldItem.report.version == newItem.report.version
            } else {
                oldItem == newItem
            }
        }
    }
    inner class ItemRecyclerViewAdapter : ListAdapter<ReportListItem, RecyclerView.ViewHolder>(ReportDiffCallback()) {
        private val onClickListener: View.OnClickListener = View.OnClickListener {
            showReport((it.tag as Report).id)
        }

        private var selectedId: Int? = null

        fun updateList(newList: List<Report>) {
            val oldSize = currentList.size
            val items = if (newList.isEmpty()) {
                emptyList()
            } else {
                newList.map { report -> ReportListItem.ReportData(report) } + ReportListItem.ClearButton
            }
            submitList(items) {
                val newSize = itemCount
                if (newSize > 0)
                    notifyItemChanged(0)
                if (oldSize > 1) {
                    val oldPenultimate = oldSize - 2
                    if (oldPenultimate < newSize)
                        notifyItemChanged(oldPenultimate)
                }
                if (newSize > 1)
                    notifyItemChanged(newSize - 2)
            }
        }

        fun setSelectedId(id: Int?) {
            val oldId = selectedId
            selectedId = id
            currentList.forEachIndexed { index, item ->
                if (item is ReportListItem.ReportData) {
                    if (item.report.id == oldId || item.report.id == id) {
                        notifyItemChanged(index)
                    }
                }
            }
        }

        override fun getItemViewType(position: Int): Int {
            return when (getItem(position)) {
                is ReportListItem.ReportData -> ITEM_VIEW_TYPE
                is ReportListItem.ClearButton -> BUTTON_VIEW_TYPE
            }
        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
            if (viewType == ITEM_VIEW_TYPE) {
                val binding = ReportListContentBinding.inflate(layoutInflater, parent, false)
                return ListViewHolder(binding)
            } else {
                val binding = ClearButtonBinding.inflate(layoutInflater, parent, false)
                return ButtonViewHolder(binding)
            }
        }

        override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {
            when (val item = getItem(position)) {
                is ReportListItem.ReportData -> {
                    if (holder is ListViewHolder) {
                        val report: Report = item.report
                        holder.name.text = report.filename
                        holder.id = report.id
                        with(holder.itemView) {
                            tag = report
                            setOnClickListener(onClickListener)
                        }
                        holder.itemView.isActivated = (report.id == selectedId)
                    }
                }
                is ReportListItem.ClearButton -> {
                    if (holder is ButtonViewHolder) {
                        with(holder.itemView) {
                            setOnClickListener {
                                reportModel.deleteAllReports()
                                intent.putExtra(Core.ARG_REPORT_ID, -1)
                                if (twoPane) {
                                    val fragment =
                                        supportFragmentManager.findFragmentById(R.id.report_detail_container)
                                    if (fragment != null) {
                                        supportFragmentManager
                                            .beginTransaction()
                                            .detach(fragment)
                                            .commit()

                                        title = getString(R.string.app_name)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        inner class ListViewHolder(binding: ReportListContentBinding) : RecyclerView.ViewHolder(binding.root) {
            val name: TextView = binding.nameText
            var id: Int = -1

            init {
                binding.deleteButton.setOnClickListener {
                    if (id != -1)
                        deleteReport(id)
                }
            }
        }

        inner class ButtonViewHolder(val binding: ClearButtonBinding) : RecyclerView.ViewHolder(binding.root)
    }

    companion object {
        const val READ_EXTERNAL_STORAGE_PERMISSION_REQUEST = 50
        const val ACCESS_MEDIA_LOCATION_PERMISSION_REQUEST_OPENFILE = 60
        const val ACCESS_MEDIA_LOCATION_PERMISSION_REQUEST_SENDFILE = 61
        const val OPEN_INTENT_PROCESSED = "net.mediaarea.mediainfo.internal.tag.Intent.Processed"
        const val ITEM_VIEW_TYPE = 0
        const val BUTTON_VIEW_TYPE = 1
    }
}
