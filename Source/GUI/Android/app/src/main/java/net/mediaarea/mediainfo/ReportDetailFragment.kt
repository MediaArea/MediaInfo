/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import java.util.Locale

import androidx.activity.result.contract.ActivityResultContracts
import androidx.core.content.edit
import androidx.core.text.htmlEncode
import androidx.core.view.MenuHost
import androidx.core.view.MenuProvider
import androidx.documentfile.provider.DocumentFile
import androidx.fragment.app.Fragment
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.lifecycleScope
import androidx.preference.PreferenceManager.getDefaultSharedPreferences

import android.os.Bundle
import android.content.SharedPreferences
import android.content.Context
import android.view.*
import android.widget.Toast

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

import net.mediaarea.mediainfo.databinding.ReportDetailBinding


class ReportDetailFragment : Fragment() {
    private lateinit var activityListener: ReportActivityListener
    private var sharedPreferences: SharedPreferences? = null
    private var _binding: ReportDetailBinding? = null
    private val binding get() = _binding!!
    private var currentViewMode: String = "HTML"
    var reportId: Int? = null

    private val saveFile =
        registerForActivityResult(ActivityResultContracts.OpenDocumentTree()) { uri ->
            val id = reportId
            if (uri == null || id == null) {
                onError()
                return@registerForActivityResult
            }

            lifecycleScope.launch {
                val contextReference = context?.applicationContext ?: return@launch
                val isSavedSuccessfully = saveReport(contextReference, uri, id)
                if (!isSavedSuccessfully)
                    onError()
            }
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        arguments?.let {
            if (it.containsKey(Core.ARG_REPORT_ID)) {
                val newId: Int = it.getInt(Core.ARG_REPORT_ID)
                if (newId != -1)
                    reportId = newId
            }
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)

        try {
            activityListener = activity as ReportActivityListener
        } catch (_: Throwable) {
            throw ClassCastException("$activity must implement ReportActivityListener")
        }

        sharedPreferences = getDefaultSharedPreferences(context)
        val oldSharedPreferences = activity?.getSharedPreferences(
            getString(R.string.preferences_key),
            Context.MODE_PRIVATE
        )
        val key = getString(R.string.preferences_view_key)

        if (sharedPreferences?.contains(key) == false && oldSharedPreferences?.contains(key) == true) {
            sharedPreferences?.edit {
                putString(key, oldSharedPreferences.getString(key, "HTML"))
            }
        }

        currentViewMode = sharedPreferences?.getString(key, "HTML") ?: "HTML"
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        _binding = ReportDetailBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        showReport()

        // Setup Menus
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
                        isChecked = (current.name == currentViewMode)
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
                        if (currentViewMode != current.name) {
                            currentViewMode = current.name

                            // Save new default
                            sharedPreferences?.edit {
                                putString(
                                    getString(R.string.preferences_view_key),
                                    currentViewMode
                                )
                            }
                            // Refresh
                            parentFragmentManager.fragments.forEach { fragmentInstance ->
                                val detailFragment = fragmentInstance as? ReportDetailFragment
                                if (detailFragment != null) {
                                    // Set the new layout rendering parameter
                                    detailFragment.currentViewMode = current.name
                                    // Fire async data load instantly if layout context is attached
                                    if (detailFragment.isAdded) {
                                        detailFragment.showReport()
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

    private fun showReport() {
        val id = reportId ?: return
        val mode = currentViewMode
        // Show the report
        viewLifecycleOwner.lifecycleScope.launch {
            val reportObject = activityListener.getReportViewModel().getReport(id)
            if (reportObject == null || reportObject.report.isEmpty()) {
                parentFragmentManager.beginTransaction()
                    .remove(this@ReportDetailFragment)
                    .commitAllowingStateLoss()
                return@launch
            }
            val reportContent = withContext(Dispatchers.IO) {
                Core.convertReport(reportObject.report, mode)
            }
            var content = ""
            if (mode != "HTML") {
                content += "<html><head>" +
                        "<style>:root { color-scheme: var(--color-scheme, light); } @media (prefers-color-scheme: dark) { :root { --color-scheme: dark; } }</style>" +
                        "</head><body><pre>"
                content += reportContent.replace("\t", "    ").htmlEncode()
                content += "</pre></body></html>"
            } else {
                content += reportContent
            }
            if (_binding != null)
                binding.reportDetail.loadDataWithBaseURL(
                    null,
                    content,
                    "text/html",
                    "utf-8",
                    null
                )
        }
    }

    private suspend fun saveReport(
        context: Context,
        treeUri: android.net.Uri,
        id: Int
    ): Boolean =
        withContext(Dispatchers.IO) {
            val reportObject = activityListener.getReportViewModel().getReport(id)
            if (reportObject == null || reportObject.report.isEmpty())
                return@withContext false
            val directory = DocumentFile.fromTreeUri(context, treeUri)
            if (directory == null || !directory.canWrite())
                return@withContext false
            val reportText = Core.convertReport(reportObject.report, currentViewMode, true)
            val filename = String.format(Locale.US, "%s.%s", reportObject.filename, currentViewMode)
            val mime = Core.views.find { it.name == currentViewMode }?.mime ?: "text/plain"

            return@withContext try {
                val document = directory.createFile(mime, filename) ?: return@withContext false
                context.contentResolver.openOutputStream(document.uri)?.use { ostream ->
                    ostream.write(reportText.toByteArray())
                    ostream.flush()
                    true
                } ?: false
            } catch (_: Exception) {
                false
            }
        }

    private fun onError() {
        activity?.applicationContext?.let {
            Toast.makeText(it, R.string.error_write_text, Toast.LENGTH_LONG).show()
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
