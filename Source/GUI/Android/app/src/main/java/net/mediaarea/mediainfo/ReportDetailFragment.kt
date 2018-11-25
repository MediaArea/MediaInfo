/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import java.io.OutputStream
import java.io.File

import androidx.fragment.app.Fragment
import androidx.documentfile.provider.DocumentFile

import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.app.Activity
import android.content.SharedPreferences
import android.content.Context
import android.content.Intent
import android.text.TextUtils
import android.view.*

import com.google.android.material.snackbar.Snackbar

import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.disposables.CompositeDisposable
import io.reactivex.schedulers.Schedulers

import com.github.angads25.filepicker.model.DialogConfigs
import com.github.angads25.filepicker.model.DialogProperties
import com.github.angads25.filepicker.view.FilePickerDialog

import kotlinx.android.synthetic.main.report_detail.view.*

class ReportDetailFragment : Fragment() {
    companion object {
        const val ARG_REPORT_ID: String = "id"
        const val SAVE_FILE_REQUEST_CODE: Int = 1
    }

    private val disposable: CompositeDisposable = CompositeDisposable()
    private lateinit var activityListener: ReportActivityListener
    private var sharedPreferences: SharedPreferences? = null
    private var view: String = "HTML"
    var id: Int? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        arguments?.let {
            if (it.containsKey(ARG_REPORT_ID)) {
                val newId: Int = it.getInt(ARG_REPORT_ID)
                if (newId != -1)
                    id = newId
            }
        }

        setHasOptionsMenu(true)
    }

    override fun onAttach(context: Context?) {
        super.onAttach(context)

        try {
            activityListener = activity as ReportActivityListener
        } catch (e: ClassCastException) {
            throw ClassCastException(activity.toString() + " must implement ReportActivityListener")
        }

        sharedPreferences = activity?.getSharedPreferences(getString(R.string.preferences_key), Context.MODE_PRIVATE)

        sharedPreferences?.getString(getString(R.string.preferences_view_key), "HTML").let {
            if (it != null)
                view = it
        }
    }

    override fun onStop() {
        super.onStop()

        // clear all the subscription
        disposable.clear()
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?,
                              savedInstanceState: Bundle?): View? {
        val rootView = inflater.inflate(R.layout.report_detail, container, false)
        // Show the report
        id?.let { id ->
            disposable.add(activityListener.getReportViewModel().getReport(id)
                    .subscribeOn(Schedulers.io())
                    .observeOn(AndroidSchedulers.mainThread())
                    .subscribe {
                        activity?.title = it.filename

                        val report: String = Core.convertReport(it.report, view)
                        var content = ""
                        if (view != "HTML") {
                            content += "<html><body><pre>"
                            content += TextUtils.htmlEncode(report.replace("\t", "    "))
                            content += "</pre></body></html>"
                        } else {
                            content+=report
                        }

                        rootView.report_detail.loadDataWithBaseURL(null, content, "text/html", "utf-8", null)
             })
        }

        return rootView
    }

    override fun onCreateOptionsMenu(menu: Menu, inflater: MenuInflater) {
        inflater.inflate(R.menu.menu_detail, menu)

        menu.findItem(R.id.action_export_report).let {
            it.setOnMenuItemClickListener {
                if (Build.VERSION.SDK_INT >= 21) {
                    val intent = Intent(Intent.ACTION_OPEN_DOCUMENT_TREE)
                    startActivityForResult(intent, SAVE_FILE_REQUEST_CODE)
                } else {
                    if (Environment.getExternalStorageState() == Environment.MEDIA_MOUNTED) {
                        val properties = DialogProperties()
                        properties.selection_mode = DialogConfigs.SINGLE_MODE
                        properties.selection_type = DialogConfigs.DIR_SELECT
                        properties.root = File(DialogConfigs.DEFAULT_DIR)
                        properties.error_dir = File(DialogConfigs.DEFAULT_DIR)
                        properties.offset = File(DialogConfigs.DEFAULT_DIR)
                        properties.extensions = null

                        val dialog = FilePickerDialog(context, properties)
                        dialog.setTitle(R.string.export_title)

                        dialog.setDialogSelectionListener { select: Array<String> ->
                            if (select.isEmpty())
                                onError()

                            id.let {id ->
                                if (id == null) {
                                    onError()
                                } else {
                                    disposable
                                            .add(activityListener.getReportViewModel().getReport(id)
                                                    .subscribeOn(Schedulers.io())
                                                    .observeOn(AndroidSchedulers.mainThread())
                                                    .subscribe { report: Report ->
                                                        if (report.report.isEmpty()) {
                                                            onError()
                                                        } else {
                                                            val directory = File(select[0])

                                                            if (!directory.canWrite()) {
                                                                onError()
                                                            } else {
                                                                saveReport(DocumentFile.fromFile(directory), report)
                                                            }
                                                        }
                                                    })
                                }
                            }

                        }

                        dialog.show()
                    } else {
                        val rootView: View? = getView()

                        // Show error message for 5 seconds
                        if (rootView != null) {
                            Snackbar.make(rootView, R.string.error_text, 5000)
                                    .setAction("Action", null)
                                    .show()
                        }
                    }
                }

                true
            }
        }

        val viewMenu: SubMenu = menu.findItem(R.id.action_change_view).subMenu

        for (current: Core.ReportView in Core.views) {
            val index: Int = Core.views.indexOf(current)
            viewMenu.add(R.id.menu_views_group, Menu.NONE, index, current.desc).setOnMenuItemClickListener { item: MenuItem ->
                val requested: String = Core.views.findLast { it.desc == item.title }?.name.orEmpty()

                if (requested.isNotEmpty() && !requested.contentEquals(view)) {
                    view = requested

                    // Save new default
                    sharedPreferences
                            ?.edit()
                            ?.putString(getString(R.string.preferences_view_key), view)
                            ?.apply()

                    // Reset view
                    fragmentManager
                            ?.beginTransaction()
                            ?.detach(this)
                            ?.attach(this)
                            ?.commit()
                }

                true
            }.setCheckable(true).setChecked(current.name == view)

            viewMenu.setGroupCheckable(R.id.menu_views_group, true, true)
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, resultData: Intent?) {
        if (resultCode == Activity.RESULT_OK) {
            when (requestCode) {
                SAVE_FILE_REQUEST_CODE -> {
                    if (resultData == null || resultData.data == null) {
                        onError()
                        return
                    }

                    id.let {
                        if (it == null) {
                            onError()
                            return
                        }

                        disposable
                                .add(activityListener.getReportViewModel().getReport(it)
                                        .subscribeOn(Schedulers.io())
                                        .observeOn(AndroidSchedulers.mainThread())
                                        .subscribe { report: Report ->
                                            if (report.report.isEmpty()) {
                                                onError()
                                            } else {
                                                val currentContext: Context? = context
                                                if (currentContext == null) {
                                                    onError()
                                                } else {
                                                    val data = resultData.data
                                                    if (data == null) {
                                                        onError()
                                                    } else {
                                                        val directory = DocumentFile.fromTreeUri(currentContext, data)

                                                        if (directory == null) {
                                                            onError()
                                                        } else {
                                                            if (!directory.canWrite()) {
                                                                onError()
                                                            } else {
                                                                saveReport(directory, report)
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        })
                    }
                }
            }
        }
    }

    private fun saveReport(directory: DocumentFile, report: Report) {
        val reportText: String = Core.convertReport(report.report, view, true)
        val filename: String = String.format("%s.%s", report.filename, view)
        val mime: String = Core.views.find { it.name == view }?.mime ?: "text/plain"

        try {
            val document = directory.createFile(mime, filename)

            if (document == null) {
                onError()
            } else {
                val ostream: OutputStream? = context?.contentResolver?.openOutputStream(document.uri)

                if (ostream == null) {
                    onError()
                } else {
                    ostream.write(reportText.toByteArray())
                    ostream.flush()
                    ostream.close()
                }
            }
        } catch (e: Exception) {
            onError()
        }
    }

    private fun onError() {
        val rootView: View? = getView()

        // Show error message for 5 seconds
        if (rootView != null) {
            Snackbar.make(rootView, R.string.error_text, 5000)
                    .setAction("Action", null)
                    .show()
        }
    }
}
