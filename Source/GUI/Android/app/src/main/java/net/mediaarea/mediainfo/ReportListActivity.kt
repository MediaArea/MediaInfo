/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import kotlin.jvm.*

import java.io.File

import android.support.v7.app.AppCompatActivity
import android.support.v7.widget.RecyclerView
import android.support.v4.app.Fragment
import android.arch.lifecycle.ViewModelProviders
import android.os.Build
import android.os.Bundle
import android.os.AsyncTask
import android.os.ParcelFileDescriptor
import android.net.Uri
import android.app.Activity
import android.content.Intent
import android.database.Cursor
import android.provider.OpenableColumns
import android.widget.FrameLayout
import android.widget.TextView
import android.content.pm.PackageManager
import android.support.v4.app.ActivityCompat
import android.view.*

import io.reactivex.disposables.CompositeDisposable
import io.reactivex.schedulers.Schedulers
import io.reactivex.android.schedulers.AndroidSchedulers

import kotlinx.android.synthetic.main.activity_report_list.*
import kotlinx.android.synthetic.main.report_list_content.view.*
import kotlinx.android.synthetic.main.report_list.*
import kotlinx.android.synthetic.main.hello_layout.*

/**
 * An activity representing a list of Pings. This activity
 * has different presentations for handset and tablet-size devices. On
 * handsets, the activity presents a list of items, which when touched,
 * lead to a [ReportDetailActivity] representing
 * item details. On tablets, the activity presents the list of items and
 * item details side-by-side using two vertical panes.
 */
class ReportListActivity : AppCompatActivity(), ReportActivityListener {
    private val OPEN_FILE_REQUEST_CODE = 40
    private val READ_EXTERNAL_STORAGE_PERMISSION_REQUEST_CODE = 50

    private lateinit var reportModel: ReportViewModel
    private var disposable: CompositeDisposable = CompositeDisposable()
    private var twoPane: Boolean = false
    private var reports: List<Report> = listOf()
    private var pendingFileUris: MutableList<Uri> = mutableListOf()

    inner class AddFile: AsyncTask<Uri, Int, Boolean>() {
        override fun onPreExecute() {
            super.onPreExecute()

            add_button.visibility = View.GONE

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

            add_button.visibility = View.VISIBLE

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
                        try {
                            val cursor: Cursor = contentResolver.query(uri, null, null, null, null, null)
                            // moveToFirst() returns false if the cursor has 0 rows
                            if (cursor.moveToFirst()) {
                                // DISPLAY_NAME is provider-specific, and might not be the file name
                                displayName = cursor.getString(cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME))
                                cursor.close()
                            }
                        } catch (e: Exception) {}
                        try {
                            fd = contentResolver.openFileDescriptor(uri, "r")
                        } catch (e: Exception) {}
                    }
                    "file" -> {
                        val file = File(uri.path)
                        try {
                            fd = ParcelFileDescriptor.open(file, ParcelFileDescriptor.MODE_READ_ONLY)
                        } catch (e: Exception) {}

                        displayName = uri.lastPathSegment
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
            READ_EXTERNAL_STORAGE_PERMISSION_REQUEST_CODE -> {
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
            val action: String = intent.action
            val uri: Uri? = intent.data
            if (action == Intent.ACTION_VIEW && uri != null) {
                if (uri.scheme == "file") {
                    if (Build.VERSION.SDK_INT >= 23) {
                        if (checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                            pendingFileUris.add(uri)
                            ActivityCompat.requestPermissions(this@ReportListActivity,
                                    arrayOf(android.Manifest.permission.READ_EXTERNAL_STORAGE),
                                    READ_EXTERNAL_STORAGE_PERMISSION_REQUEST_CODE)
                            return
                        }
                    }
                }

                AddFile().execute(uri)
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
            val fragment: Fragment? = supportFragmentManager.findFragmentById(R.id.report_detail_container)
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

        menu?.findItem(R.id.action_about).let {
            it?.setOnMenuItemClickListener {
                val intent = Intent(this,  AboutActivity::class.java)
                startActivity(intent)

                true
            }
        }

        return super.onCreateOptionsMenu(menu)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, resultData: Intent?) {
        if (resultCode == Activity.RESULT_OK) {
            when (requestCode) {
                OPEN_FILE_REQUEST_CODE -> {
                    if (resultData == null)
                        return

                    if (resultData.clipData != null) {
                        val uris: Array<Uri> = Array(resultData.clipData.itemCount) {
                            resultData.clipData.getItemAt(it).uri
                        }
                        AddFile().execute(*(uris))
                    } else if (resultData.data != null) {
                        AddFile().execute(resultData.data)
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

        val viewModelFactory = Injection.provideViewModelFactory(this)
        ViewModelProviders.of(this, viewModelFactory).get(ReportViewModel::class.java)
        reportModel = ViewModelProviders.of(this, viewModelFactory).get(ReportViewModel::class.java)

        add_button.setOnClickListener {
            val intent = Intent(Intent.ACTION_OPEN_DOCUMENT)

            intent.addCategory(Intent.CATEGORY_OPENABLE)
            intent.type = "*/*"
            intent.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, true)

            startActivityForResult(intent, OPEN_FILE_REQUEST_CODE)
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
                    } else {
                        for (i: Int in rootLayout.childCount downTo 1) {
                            if (rootLayout.getChildAt(i - 1).id == R.id.hello_layout)
                                rootLayout.removeViewAt(i - 1)
                        }
                        rootLayout.removeView(hello_layout)
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
}
