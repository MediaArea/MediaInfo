/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import kotlin.jvm.*

import java.io.File

import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.RecyclerView
import androidx.lifecycle.ViewModelProvider
import androidx.core.app.ActivityCompat
import androidx.appcompat.app.AppCompatDelegate

import android.os.Build
import android.os.Bundle
import android.os.AsyncTask
import android.os.Environment
import android.os.ParcelFileDescriptor
import android.net.Uri
import android.app.Activity
import android.content.Intent
import android.database.Cursor
import android.provider.OpenableColumns
import android.widget.FrameLayout
import android.widget.TextView
import android.content.Context
import android.widget.Toast
import android.content.SharedPreferences
import android.content.pm.PackageManager
import android.content.res.Configuration
import android.content.res.Resources
import android.view.*
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.updatePadding
import androidx.preference.PreferenceManager.getDefaultSharedPreferences
import io.reactivex.disposables.CompositeDisposable
import io.reactivex.schedulers.Schedulers
import io.reactivex.android.schedulers.AndroidSchedulers

import com.yariksoffice.lingver.Lingver
import java.io.BufferedReader
import java.util.*

import  net.mediaarea.mediainfo.databinding.ActivityReportListBinding
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
    private var disposable: CompositeDisposable = CompositeDisposable()
    private var twoPane: Boolean = false
    private var reports: List<Report> = listOf()
    private var pendingFileUris: MutableList<Uri> = mutableListOf()

    inner class AddFile: AsyncTask<Uri, Int, Boolean>() {
        override fun onPreExecute() {
            super.onPreExecute()

            activityReportListBinding.addButton.hide()
            val rootLayout: FrameLayout = activityReportListBinding.frameLayout
            var found = false
            for (i: Int in rootLayout.childCount downTo 1) {
                if (rootLayout.getChildAt(i - 1).id == R.id.spinner_layout)
                    found = true
            }

            if (!found)
                View.inflate(this@ReportListActivity, R.layout.spinner_layout, rootLayout)
        }

        override fun onPostExecute(result: Boolean?) {
            super.onPostExecute(result)

            activityReportListBinding.addButton.show()

            val rootLayout: FrameLayout = activityReportListBinding.frameLayout
            for (i: Int in rootLayout.childCount downTo 1) {
                if (rootLayout.getChildAt(i - 1).id == R.id.spinner_layout)
                    rootLayout.removeViewAt(i - 1)
            }
        }

        override fun doInBackground(vararg params: Uri?): Boolean {
            for (uri: Uri? in params) {
                if (uri == null) {
                    continue
                }

                var fd: ParcelFileDescriptor? = null
                var displayName = ""

                when (uri.scheme) {
                    "content" -> {
                        try {
                            val cursor: Cursor? = contentResolver.query(uri, null, null, null, null, null)
                            // moveToFirst() returns false if the cursor has 0 rows
                            if (cursor != null && cursor.moveToFirst()) {
                                // DISPLAY_NAME is provider-specific, and might not be the file name
                                val column = cursor.getColumnIndexOrThrow(OpenableColumns.DISPLAY_NAME)
                                if (column >= 0) {
                                    displayName = cursor.getString(column)
                                }
                                cursor.close()
                            }
                        } catch (e: Exception) {
                        }
                        try {
                            fd = contentResolver.openFileDescriptor(uri, "r")
                        } catch (e: Exception) {}
                    }
                    "file" -> {
                        val file = File(uri.path.orEmpty())
                        try {
                            fd = ParcelFileDescriptor.open(file, ParcelFileDescriptor.MODE_READ_ONLY)
                        } catch (e: Exception) {}

                        displayName = uri.lastPathSegment.orEmpty()
                    }
                }

                if (fd == null) {
                    continue
                }

                val report: ByteArray = Core.createReport(fd.detachFd(), displayName)

                disposable.add(reportModel.insertReport(Report(0, displayName, report, Core.version))
                        .subscribeOn(Schedulers.io())
                        .observeOn(AndroidSchedulers.mainThread())
                        .doOnComplete {
                            // Don't go to report view when opening multiples files
                            if (params.size == 1) {
                                disposable.add(reportModel.getLastId()
                                        .subscribeOn(Schedulers.io())
                                        .observeOn(AndroidSchedulers.mainThread())
                                        .doOnSuccess {
                                            showReport(it)
                                        }.subscribe())
                            }
                        }.subscribe())
            }
            return true
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        when (requestCode) {
            READ_EXTERNAL_STORAGE_PERMISSION_REQUEST -> {
                if ((grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED)) {
                    AddFile().execute(*(pendingFileUris.toTypedArray()))
                    pendingFileUris.clear()
                } else {
                    // Abort all pending request
                    pendingFileUris.clear()
                }
            }
        }
    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
    }

    override fun onNewIntent(intent: Intent) {
        super.onNewIntent(intent)
        setIntent(intent)
        handleIntent(intent)
    }

    private fun handleUri(uri: Uri) {
        if (uri.scheme == "file") {
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
        AddFile().execute(uri)
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
                    val uri = intent.getParcelableExtra<Uri>(Intent.EXTRA_STREAM)
                    if (uri != null) {
                        handleUri(uri)
                        intent.putExtra(OPEN_INTENT_PROCESSED, true)
                    }
                }
                Intent.ACTION_SEND_MULTIPLE -> {
                    val uriList = intent.getParcelableArrayListExtra<Uri>(Intent.EXTRA_STREAM)
                    if (uriList != null) {
                        for (i in uriList) {
                            handleUri(i)
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
                val oldSharedPreferences = getSharedPreferences(getString(R.string.preferences_key), Context.MODE_PRIVATE)
                if (oldSharedPreferences?.contains(key) == true) {
                    oldSharedPreferences.getString(key, "").let {
                        when (it) {
                            "ON" -> {
                                oldSharedPreferences.edit()?.remove(key)
                                sharedPreferences.edit()?.putString(key, "on")?.apply()
                            }
                            "OFF" -> {
                                oldSharedPreferences.edit()?.remove(key)
                                sharedPreferences.edit()?.putString(key, "off")?.apply()

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
                                sharedPreferences.edit()?.remove(key)
                                sharedPreferences.edit()?.putString(key, "off")?.apply()
                            }
                            true -> {
                                sharedPreferences.edit()?.remove(key)
                                sharedPreferences.edit()?.putString(key, "on")?.apply()
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
        catch (error: Exception) {
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
                        Locale(language[0], language[1])
                    } else {
                        Locale(language[0])
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

            disposable.add(reportModel.getReport(id)
                    .subscribeOn(Schedulers.io())
                    .observeOn(AndroidSchedulers.mainThread())
                    .doOnSuccess {
                        title = it.filename
                    }.subscribe())
        } else {
            val intent = Intent(this@ReportListActivity, ReportDetailActivity::class.java)
            intent.putExtra(Core.ARG_REPORT_ID, id)

            startActivityForResult(intent, SHOW_REPORT_REQUEST)
        }
    }

    fun deleteReport(id: Int) {
        disposable.add(reportModel
                .deleteReport(id)
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe())

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
                            intent.data = Uri.parse(getString(R.string.subscription_manage_url).replace('|', '&'))
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
                        startActivityForResult(intent, SUBSCRIBE_REQUEST)

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

    override fun onActivityResult(requestCode: Int, resultCode: Int, resultData: Intent?) {
        super.onActivityResult(requestCode, resultCode, resultData)

        if (resultCode == Activity.RESULT_OK) {
            when (requestCode) {
                OPEN_FILE_REQUEST -> {
                    if (resultData == null)
                        return

                    val clipData = resultData.clipData
                    if (clipData != null) {
                        val uris: Array<Uri> = Array(clipData.itemCount) {
                            clipData.getItemAt(it).uri
                        }
                        AddFile().execute(*(uris))
                    } else if (resultData.data != null) {
                        AddFile().execute(resultData.data)
                    }
                }
                SUBSCRIBE_REQUEST -> {
                    Toast.makeText(applicationContext, R.string.thanks_text, Toast.LENGTH_SHORT).show()
                }
                SHOW_REPORT_REQUEST -> {
                    if (resultData!=null) {
                        val id = resultData.getIntExtra(Core.ARG_REPORT_ID, -1)
                        if (id!=-1 && twoPane) {
                            showReport(id)
                        }
                    }
                }
            }
        }
    }

    override fun getReportViewModel(): ReportViewModel {
        return reportModel
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        activityReportListBinding = ActivityReportListBinding.inflate(layoutInflater)
        helloLayoutBinding = HelloLayoutBinding.inflate(layoutInflater)

        ViewCompat.setOnApplyWindowInsetsListener(activityReportListBinding.appBar) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updatePadding(
                left = bars.left,
                top = bars.top,
                right = bars.right,
            )
            WindowInsetsCompat.CONSUMED
        }
        ViewCompat.setOnApplyWindowInsetsListener(activityReportListBinding.frameLayout) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updatePadding(
                bottom = bars.bottom,
            )
            WindowInsetsCompat.CONSUMED
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

        activityReportListBinding.addButton.setOnClickListener {
            val intent = Intent(Intent.ACTION_GET_CONTENT)

            intent.addCategory(Intent.CATEGORY_OPENABLE)
            intent.type = "*/*"
            intent.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, true)

            startActivityForResult(intent, OPEN_FILE_REQUEST)
        }
        activityReportListBinding.reportListLayout.clearBtn.setOnClickListener {
            disposable.add(reportModel.deleteAllReports()
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe {
                    intent.putExtra(Core.ARG_REPORT_ID, -1)
                    if (twoPane) {
                        val fragment = supportFragmentManager.findFragmentById(R.id.report_detail_container)
                        if (fragment != null) {
                            supportFragmentManager
                                .beginTransaction()
                                .detach(fragment)
                                .commit()

                                title = getString(R.string.app_name)
                        }
                    }
            })
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

        setupRecyclerView(activityReportListBinding.reportListLayout.reportList)

        onNewIntent(intent)
    }

    override fun onStart() {
        super.onStart()

        disposable.add(reportModel.getAllReports()
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe {
                    reports = it
                    setupRecyclerView(activityReportListBinding.reportListLayout.reportList)

                    val rootLayout: FrameLayout = findViewById(R.id.frame_layout)
                    if (reports.isEmpty()) {
                        var found = false
                        for (i: Int in rootLayout.childCount downTo 1) {
                            if (rootLayout.getChildAt(i - 1).id == R.id.hello_layout)
                                found = true
                        }

                        if (!found)
                            View.inflate(this, R.layout.hello_layout, rootLayout)

                        activityReportListBinding.reportListLayout.clearBtn.visibility = View.INVISIBLE
                    } else {
                        for (i: Int in rootLayout.childCount downTo 1) {
                            if (rootLayout.getChildAt(i - 1).id == R.id.hello_layout)
                                rootLayout.removeViewAt(i - 1)
                        }
                        rootLayout.removeView(helloLayoutBinding.root)
                        activityReportListBinding.reportListLayout.clearBtn.visibility = View.VISIBLE


                    }
                })

        if (twoPane && intent.getIntExtra(Core.ARG_REPORT_ID, -1)!=-1) {
            showReport(intent.getIntExtra(Core.ARG_REPORT_ID, -1))
        }
    }

    override fun onStop() {
        super.onStop()

        // clear all the subscription
        disposable.clear()
    }

    private fun setupRecyclerView(recyclerView: RecyclerView) {
        recyclerView.adapter = ItemRecyclerViewAdapter()
        recyclerView.isNestedScrollingEnabled = false
        recyclerView.adapter?.notifyDataSetChanged()
    }

    inner class ItemRecyclerViewAdapter : RecyclerView.Adapter<ItemRecyclerViewAdapter.ViewHolder>() {
        private val onClickListener: View.OnClickListener = View.OnClickListener {
            showReport((it.tag as Report).id)
        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
            val binding = ReportListContentBinding.inflate(layoutInflater, parent, false)
            return ViewHolder(binding)
        }

        override fun onBindViewHolder(holder: ViewHolder, position: Int) {
            val report: Report = reports[position]
            holder.name.text = report.filename
            holder.id = report.id

            with(holder.itemView) {
                tag = report
                setOnClickListener(onClickListener)
            }
        }

        override fun getItemCount(): Int {
            return reports.size
        }

        inner class ViewHolder(binding: ReportListContentBinding) : RecyclerView.ViewHolder(binding.root) {
            val name: TextView = binding.nameText
            var id: Int = -1

            init {
                binding.deleteButton.setOnClickListener {
                    if (id != -1)
                        deleteReport(id)
                }
            }
        }
    }

    companion object {
        const val SHOW_REPORT_REQUEST = 20
        const val SUBSCRIBE_REQUEST = 30
        const val OPEN_FILE_REQUEST = 40
        const val READ_EXTERNAL_STORAGE_PERMISSION_REQUEST = 50
        const val OPEN_INTENT_PROCESSED = "net.mediaarea.mediainfo.internal.tag.Intent.Processed"
    }
}
