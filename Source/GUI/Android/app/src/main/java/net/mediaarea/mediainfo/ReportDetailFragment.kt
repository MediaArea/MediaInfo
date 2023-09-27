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
import androidx.preference.PreferenceManager.getDefaultSharedPreferences

import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.app.Activity
import android.content.SharedPreferences
import android.content.Context
import android.content.Intent
import android.text.TextUtils
import android.view.*
import android.widget.Toast

import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.disposables.CompositeDisposable
import io.reactivex.schedulers.Schedulers

import com.github.angads25.filepicker.model.DialogConfigs
import com.github.angads25.filepicker.model.DialogProperties
import com.github.angads25.filepicker.view.FilePickerDialog
import net.mediaarea.mediainfo.databinding.ReportDetailBinding


class ReportDetailFragment : Fragment() {
    companion object {
        const val SAVE_FILE_REQUEST_CODE: Int = 1
    }

    private val disposable: CompositeDisposable = CompositeDisposable()
    private lateinit var activityListener: ReportActivityListener
    private var sharedPreferences: SharedPreferences? = null
    private lateinit var reportDetailBinding: ReportDetailBinding
    private var view: String = "HTML"
    var id: Int? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        arguments?.let {
            if (it.containsKey(Core.ARG_REPORT_ID)) {
                val newId: Int = it.getInt(Core.ARG_REPORT_ID)
                if (newId != -1)
                    id = newId
            }
        }

        setHasOptionsMenu(true)
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)

        reportDetailBinding = ReportDetailBinding.inflate(layoutInflater)

        try {
            activityListener = activity as ReportActivityListener
        } catch (_: Throwable) {
            throw ClassCastException(activity.toString() + " must implement ReportActivityListener")
        }

        sharedPreferences = getDefaultSharedPreferences(context)
        val oldSharedPreferences = activity?.getSharedPreferences(getString(R.string.preferences_key), Context.MODE_PRIVATE)
        val key = getString(R.string.preferences_view_key)

        if (sharedPreferences?.contains(key) == false && oldSharedPreferences?.contains(key) == true) {
            sharedPreferences?.edit()
                             ?.putString(key, oldSharedPreferences.getString(key, "HTML"))
                             ?.apply()
        }

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
                              savedInstanceState: Bundle?): View {

        //val rootView = inflater.inflate(R.layout.report_detail, container, false)
        // Show the report
        id?.let { id ->
            disposable.add(activityListener.getReportViewModel().getReport(id)
                    .subscribeOn(Schedulers.io())
                    .observeOn(AndroidSchedulers.mainThread())
                    .doOnSuccess {
                        val report: String = Core.convertReport(it.report, view)
                        var content = ""
                        if (view != "HTML") {
                            content += "<html><body><pre>"
                            content += TextUtils.htmlEncode(report.replace("\t", "    "))
                            content += "</pre></body></html>"
                        } else {
                            content+=report
                        }

                        val background=resources.getString(0+R.color.background).removeRange(1, 3)
                        val foreground=resources.getString(0+R.color.foreground).removeRange(1, 3)
                        content = content.replace("<body>", "<body style=\"background-color: ${background}; color: ${foreground};\">")

                        reportDetailBinding.reportDetail.loadDataWithBaseURL(null, content, "text/html", "utf-8", null)
             }.subscribe())
        }

        return reportDetailBinding.root
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
                        onError()
                    }
                }

                true
            }
        }

        val viewMenu: SubMenu = menu.findItem(R.id.action_change_view).subMenu ?: return
        for (current: Core.ReportView in Core.views) {
            val index: Int = Core.views.indexOf(current)
            var desc = current.desc
            if (desc == "Text") {
                desc = resources.getString(R.string.text_output_desc)
            }

            viewMenu.add(R.id.menu_views_group, Menu.NONE, index, desc).setOnMenuItemClickListener {
                if (view != current.name) {
                    view = current.name

                    // Save new default
                    sharedPreferences
                            ?.edit()
                            ?.putString(getString(R.string.preferences_view_key), view)
                            ?.apply()

                    // Reset view
                    parentFragmentManager.fragments.forEach {
                        val fragment = it as? ReportDetailFragment
                        if (fragment!=null) {
                            fragment.view = current.name
                            if (fragment.isAdded) {
                                parentFragmentManager
                                        .beginTransaction()
                                        .detach(it)
                                        .commit()
                                parentFragmentManager
                                        .beginTransaction()
                                        .attach(it)
                                        .commit()
                            }
                        }
                    }
                }

                true
            }.setCheckable(true).isChecked = (current.name == view)

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
        val applicationContext = activity?.applicationContext
        if (applicationContext!=null) {
            val toast = Toast.makeText(applicationContext, R.string.error_write_text, Toast.LENGTH_LONG)
            toast.show()
        }
    }
}
