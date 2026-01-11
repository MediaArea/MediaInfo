/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import java.io.OutputStream
import java.util.Locale

import androidx.activity.result.contract.ActivityResultContracts
import androidx.core.content.edit
import androidx.core.text.htmlEncode
import androidx.core.view.MenuHost
import androidx.core.view.MenuProvider
import androidx.documentfile.provider.DocumentFile
import androidx.fragment.app.Fragment
import androidx.lifecycle.Lifecycle
import androidx.preference.PreferenceManager.getDefaultSharedPreferences

import android.os.Bundle
import android.content.SharedPreferences
import android.content.Context
import android.view.*
import android.widget.Toast

import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.disposables.CompositeDisposable
import io.reactivex.schedulers.Schedulers

import net.mediaarea.mediainfo.databinding.ReportDetailBinding


class ReportDetailFragment : Fragment() {
    private val disposable: CompositeDisposable = CompositeDisposable()
    private lateinit var activityListener: ReportActivityListener
    private var sharedPreferences: SharedPreferences? = null
    private lateinit var reportDetailBinding: ReportDetailBinding
    private var view: String = "HTML"
    var id: Int? = null

    private val saveFile = registerForActivityResult(ActivityResultContracts.OpenDocumentTree()) { uri ->
        if (uri == null) {
            onError()
            return@registerForActivityResult
        }

        id.let {
            if (it == null) {
                onError()
                return@let
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
                                val directory = DocumentFile.fromTreeUri(currentContext, uri)

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
                    })
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        arguments?.let {
            if (it.containsKey(Core.ARG_REPORT_ID)) {
                val newId: Int = it.getInt(Core.ARG_REPORT_ID)
                if (newId != -1)
                    id = newId
            }
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)

        reportDetailBinding = ReportDetailBinding.inflate(layoutInflater)

        try {
            activityListener = activity as ReportActivityListener
        } catch (_: Throwable) {
            throw ClassCastException("$activity must implement ReportActivityListener")
        }

        sharedPreferences = getDefaultSharedPreferences(context)
        val oldSharedPreferences = activity?.getSharedPreferences(getString(R.string.preferences_key), Context.MODE_PRIVATE)
        val key = getString(R.string.preferences_view_key)

        if (sharedPreferences?.contains(key) == false && oldSharedPreferences?.contains(key) == true) {
            sharedPreferences?.edit {
                putString(key, oldSharedPreferences.getString(key, "HTML"))
            }
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
                            content += "<html><head>" +
                                    "<style>:root { color-scheme: var(--color-scheme, light); } @media (prefers-color-scheme: dark) { :root { --color-scheme: dark; } }</style>" +
                                    "</head><body><pre>"
                            content += report.replace("\t", "    ").htmlEncode()
                            content += "</pre></body></html>"
                        } else {
                            content+=report
                        }
                        reportDetailBinding.reportDetail.loadDataWithBaseURL(null, content, "text/html", "utf-8", null)
             }.subscribe())
        }

        return reportDetailBinding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val menuHost: MenuHost = requireActivity()
        menuHost.addMenuProvider(object : MenuProvider {

            override fun onCreateMenu(menu: Menu, menuInflater: MenuInflater) {
                menuInflater.inflate(R.menu.menu_detail, menu)

                val viewMenu: SubMenu = menu.findItem(R.id.action_change_view).subMenu ?: return
                Core.views.forEachIndexed { index, current ->
                    val desc = if (current.desc == "Text") {
                        resources.getString(R.string.text_output_desc)
                    } else {
                        current.desc
                    }

                    viewMenu.add(R.id.menu_views_group, index, Menu.NONE, desc).apply {
                        isCheckable = true
                        isChecked = (current.name == this@ReportDetailFragment.view)
                    }
                }

                viewMenu.setGroupCheckable(R.id.menu_views_group, true, true)
            }

            override fun onMenuItemSelected(menuItem: MenuItem): Boolean {
                return when (menuItem.itemId) {
                    R.id.action_export_report -> {
                        saveFile.launch(null)
                        true
                    }
                    else -> if (menuItem.groupId == R.id.menu_views_group) {
                        val current = Core.views[menuItem.itemId]
                        if (this@ReportDetailFragment.view != current.name) {
                            this@ReportDetailFragment.view = current.name

                            // Save new default
                            sharedPreferences?.edit {
                                putString(
                                    getString(R.string.preferences_view_key),
                                    this@ReportDetailFragment.view
                                )
                            }

                            // Reset view
                            parentFragmentManager.fragments.forEach {
                                val fragment = it as? ReportDetailFragment
                                if (fragment != null) {
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
                    } else {
                        false
                    }
                }
            }

        }, viewLifecycleOwner, Lifecycle.State.RESUMED)
    }

    private fun saveReport(directory: DocumentFile, report: Report) {
        val reportText: String = Core.convertReport(report.report, view, true)
        val filename: String = String.format(Locale.US, "%s.%s", report.filename, view)
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
        } catch (_: Exception) {
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
