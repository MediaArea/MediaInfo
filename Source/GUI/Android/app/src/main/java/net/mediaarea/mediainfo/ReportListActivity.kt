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
import androidx.lifecycle.ViewModelProviders
import androidx.lifecycle.Observer
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
import android.content.res.Resources
import android.view.*
import androidx.preference.PreferenceManager.getDefaultSharedPreferences

import io.reactivex.disposables.CompositeDisposable
import io.reactivex.schedulers.Schedulers
import io.reactivex.android.schedulers.AndroidSchedulers

import kotlinx.android.synthetic.main.activity_report_list.*
import kotlinx.android.synthetic.main.report_list_content.view.*
import kotlinx.android.synthetic.main.report_list.*
import kotlinx.android.synthetic.main.hello_layout.*

import com.github.angads25.filepicker.model.DialogConfigs
import com.github.angads25.filepicker.model.DialogProperties
import com.github.angads25.filepicker.view.FilePickerDialog
import com.yariksoffice.lingver.Lingver
import java.io.BufferedReader
import java.util.*

/**
 * An activity representing a list of Pings. This activity
 * has different presentations for handset and tablet-size devices. On
 * handsets, the activity presents a list of items, which when touched,
 * lead to a [ReportDetailActivity] representing
 * item details. On tablets, the activity presents the list of items and
 * item details side-by-side using two vertical panes.
 */
class ReportListActivity : AppCompatActivity(), ReportActivityListener {
    private lateinit var subscriptionManager: SubscriptionManager
    private lateinit var reportModel: ReportViewModel
    private var disposable: CompositeDisposable = CompositeDisposable()
    private var twoPane: Boolean = false
    private var reports: List<Report> = listOf()
    private var pendingFileUris: MutableList<Uri> = mutableListOf()

    inner class AddFile: AsyncTask<Uri, Int, Boolean>() {
        override fun onPreExecute() {
            super.onPreExecute()

            add_button.hide()

            val rootLayout: FrameLayout = findViewById(R.id.frame_layout)
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

            add_button.show()

            val rootLayout: FrameLayout = findViewById(R.id.frame_layout)
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
                        if (Build.VERSION.SDK_INT >= 19) {
                            try {
                                val cursor: Cursor? = contentResolver.query(uri, null, null, null, null, null)
                                // moveToFirst() returns false if the cursor has 0 rows
                                if (cursor != null && cursor.moveToFirst()) {
                                    // DISPLAY_NAME is provider-specific, and might not be the file name
                                    displayName = cursor.getString(cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME))
                                    cursor.close()
                                }
                            } catch (e: Exception) {
                            }
                            try {
                                fd = contentResolver.openFileDescriptor(uri, "r")
                            } catch (e: Exception) {}
                        }
                    }
                    "file" -> {
                        val file = File(uri.path)
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
                                            val id: Int = it
                                            if (twoPane) {
                                                val fragment: ReportDetailFragment = ReportDetailFragment().apply {
                                                    arguments = Bundle().apply {
                                                        putInt(ReportDetailFragment.ARG_REPORT_ID, id)
                                                    }
                                                }

                                                supportFragmentManager
                                                        .beginTransaction()
                                                        .replace(R.id.report_detail_container, fragment)
                                                        .commit()
                                            } else {
                                                val intent = Intent(this@ReportListActivity, ReportDetailActivity::class.java)
                                                intent.putExtra(ReportDetailFragment.ARG_REPORT_ID, id)

                                                startActivity(intent)
                                            }
                                        }.subscribe())
                            }
                        }.subscribe())
            }
            return true
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
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

    override fun onNewIntent(intent: Intent) {
        super.onNewIntent(intent)
        setIntent(intent)
        handleIntent(intent)
    }

    private fun handleIntent(intent: Intent) {
        if (intent.action != null) {
            val action: String? = intent.action
            val uri: Uri? = intent.data
            if (action == Intent.ACTION_VIEW && uri != null) {
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
        }
    }

    private fun applyUiMode() {
        val sharedPreferences = getDefaultSharedPreferences(this)
        val key = getString(R.string.preferences_uimode_key)
        if (sharedPreferences?.contains(key) == false) {
            val oldSharedPreferences = getSharedPreferences(getString(R.string.preferences_key), Context.MODE_PRIVATE)
            oldSharedPreferences.getString(key, "").let {
                when (it) {
                    "ON" -> {
                        sharedPreferences.edit()?.putBoolean(key, true)?.apply()
                    }
                    "OFF" -> {
                        sharedPreferences.edit()?.putBoolean(key, false)?.apply()
                    }
                    else -> {}
                }
            }
        }

        sharedPreferences?.getBoolean(getString(R.string.preferences_uimode_key), false).let {
            when (it) {
                false -> {
                    if (AppCompatDelegate.getDefaultNightMode()!=AppCompatDelegate.MODE_NIGHT_NO) {
                        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO)
                        recreate()
                    }
                }
                true -> {
                    if (AppCompatDelegate.getDefaultNightMode()!=AppCompatDelegate.MODE_NIGHT_YES) {
                        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES)
                        recreate()
                    }
                }
                /* "AUTO" -> {
                    if (AppCompatDelegate.getDefaultNightMode()!=AppCompatDelegate.MODE_NIGHT_AUTO) {
                        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_AUTO)
                        recreate()
                    }
                } */
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
                false -> {
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

    fun deleteReport(id: Int) {
        disposable.add(reportModel
                .deleteReport(id)
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe())

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


        menu?.findItem(R.id.action_subscribe)?.isEnabled=false
        subscriptionManager.ready.observe(this, Observer {
            if (menu?.findItem(R.id.action_subscribe)?.title!=getString(R.string.subscribed_text)) {
                menu?.findItem(R.id.action_subscribe)?.isEnabled = it
            }
        })

        subscriptionManager.subscribed.observe(this, Observer {
            if (it) {
                //menu?.findItem(R.id.action_settings)?.isVisible=true
                menu?.findItem(R.id.action_subscribe).let { item ->
                    item?.isEnabled=true
                    item?.title = getString(R.string.subscribed_text)

                    item?.setOnMenuItemClickListener { _ ->
                        val intent = Intent(Intent.ACTION_VIEW)
                        intent.data = Uri.parse(getString(R.string.subscription_manage_url).replace('|', '&'))
                        startActivity(intent)

                        true
                    }
                }

                if (!Core.thanked) {
                    // Show thanks message
                    Core.thanked=true
                    val toast = Toast.makeText(applicationContext, R.string.thanks_text, Toast.LENGTH_SHORT)
                    toast.show()
                }
            } else {
                menu?.findItem(R.id.action_subscribe).let { item ->
                    item?.title = getString(R.string.subscribe_text)

                    item?.setOnMenuItemClickListener { _ ->
                        val intent = Intent(this, SubscribeActivity::class.java)
                        startActivity(intent)

                        true
                    }
                }
            }
        })

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
                    if (Build.VERSION.SDK_INT >= 19) {
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
                }
            }
        }
    }

    override fun getReportViewModel(): ReportViewModel {
        return reportModel
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_report_list)

        setSupportActionBar(tool_bar)
        tool_bar.title = title

        subscriptionManager = SubscriptionManager.getInstance(application)
        lifecycle.addObserver(subscriptionManager)

        setLocale()
        subscriptionManager.subscribed.observe(this, Observer {
            if (it==true) {
                applyUiMode()
                setPrefLocale()
            }
        })

        val viewModelFactory = Injection.provideViewModelFactory(this)
        ViewModelProviders.of(this, viewModelFactory).get(ReportViewModel::class.java)
        reportModel = ViewModelProviders.of(this, viewModelFactory).get(ReportViewModel::class.java)

        add_button.setOnClickListener {
            if (Build.VERSION.SDK_INT >= 19) {
                val intent = Intent(Intent.ACTION_OPEN_DOCUMENT)

                intent.addCategory(Intent.CATEGORY_OPENABLE)
                intent.type = "*/*"
                intent.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, true)

                startActivityForResult(intent, OPEN_FILE_REQUEST)
            } else {
                if (Environment.getExternalStorageState() in setOf(Environment.MEDIA_MOUNTED, Environment.MEDIA_MOUNTED_READ_ONLY)) {
                    val properties = DialogProperties()
                    properties.selection_mode = DialogConfigs.MULTI_MODE
                    properties.selection_type = DialogConfigs.FILE_SELECT
                    properties.root = File(DialogConfigs.DEFAULT_DIR)
                    properties.error_dir = File(DialogConfigs.DEFAULT_DIR)
                    properties.offset = File(DialogConfigs.DEFAULT_DIR)
                    properties.extensions = null

                    val dialog = FilePickerDialog(this, properties)
                    dialog.setTitle(R.string.open_title)

                    dialog.setDialogSelectionListener { files: Array<String> ->
                        var uris: Array<Uri> = arrayOf()

                        files.forEach { uri: String ->
                            uris += Uri.parse("file://$uri")
                        }
                        AddFile().execute(*(uris))
                    }

                    dialog.show()
                } else {
                    val toast = Toast.makeText(applicationContext, R.string.media_error_text, Toast.LENGTH_LONG)
                    toast.show()
                }
            }
        }

        clear_btn.setOnClickListener {
            disposable.add(reportModel.deleteAllReports()
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe {
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
        if (report_detail_container != null)
            twoPane = true

        setupRecyclerView(report_list)

        onNewIntent(intent)
    }

    override fun onStart() {
        super.onStart()

        disposable.add(reportModel.getAllReports()
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe {
                    reports = it
                    setupRecyclerView(report_list)

                    val rootLayout: FrameLayout = findViewById(R.id.frame_layout)
                    if (reports.isEmpty()) {
                        var found = false
                        for (i: Int in rootLayout.childCount downTo 1) {
                            if (rootLayout.getChildAt(i - 1).id == R.id.hello_layout)
                                found = true
                        }

                        if (!found)
                            View.inflate(this, R.layout.hello_layout, rootLayout)

                        clear_btn.visibility = View.INVISIBLE
                    } else {
                        for (i: Int in rootLayout.childCount downTo 1) {
                            if (rootLayout.getChildAt(i - 1).id == R.id.hello_layout)
                                rootLayout.removeViewAt(i - 1)
                        }
                        rootLayout.removeView(hello_layout)
                        clear_btn.visibility = View.VISIBLE


                    }
                })
    }

    override fun onStop() {
        super.onStop()

        // clear all the subscription
        disposable.clear()
    }

    private fun setupRecyclerView(recyclerView: RecyclerView) {
        recyclerView.adapter = ItemRecyclerViewAdapter(this, reports, twoPane)
        recyclerView.isNestedScrollingEnabled = false
    }

    class ItemRecyclerViewAdapter(private val parentActivity: ReportListActivity,
                                         private val reports: List<Report>,
                                         private val twoPane: Boolean) :
            RecyclerView.Adapter<ItemRecyclerViewAdapter.ViewHolder>() {

        private val onClickListener: View.OnClickListener

        init {
            onClickListener = View.OnClickListener {
                val report: Report = it.tag as Report
                if (twoPane) {
                    val fragment: ReportDetailFragment = ReportDetailFragment().apply {
                        arguments = Bundle().apply {
                            putInt(ReportDetailFragment.ARG_REPORT_ID, report.id)
                        }
                    }
                    parentActivity.supportFragmentManager
                            .beginTransaction()
                            .replace(R.id.report_detail_container, fragment)
                            .commit()
                } else {
                    val intent = Intent(it.context, ReportDetailActivity::class.java).apply {
                        putExtra(ReportDetailFragment.ARG_REPORT_ID, report.id)
                    }
                    it.context.startActivity(intent)
                }
            }
        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
            val view = LayoutInflater.from(parent.context)
                    .inflate(R.layout.report_list_content, parent, false)
            return ViewHolder(view)
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

        override fun getItemCount(): Int = reports.size

        inner class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {
            val name: TextView = view.name_text
            var id: Int = -1

            init {
                view.delete_button.setOnClickListener {
                    if (id != -1)
                        parentActivity.deleteReport(id)
                }
            }
        }
    }

    companion object {
        const val OPEN_FILE_REQUEST= 40
        const val READ_EXTERNAL_STORAGE_PERMISSION_REQUEST = 50
    }
}
