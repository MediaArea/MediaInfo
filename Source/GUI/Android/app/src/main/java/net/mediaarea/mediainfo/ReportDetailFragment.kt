/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.support.v4.app.Fragment
import android.os.Bundle
import android.content.SharedPreferences
import android.content.Context
import android.content.Intent
import android.view.*

import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.disposables.CompositeDisposable
import io.reactivex.schedulers.Schedulers

import kotlinx.android.synthetic.main.report_detail.view.*

class ReportDetailFragment : Fragment() {
    companion object {
        const val ARG_REPORT_ID: String = "id"
    }

    private val disposable: CompositeDisposable = CompositeDisposable()
    private lateinit var activityListener: ReportActivityListener
    private var sharedPreferences: SharedPreferences? = null
    private var view: String = "Text"
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

        sharedPreferences?.getString(getString(R.string.preferences_view_key), "Text").let {
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
        id?.let {
            disposable.add(activityListener.getReportViewModel().getReport(it)
                    .subscribeOn(Schedulers.io())
                    .observeOn(AndroidSchedulers.mainThread())
                    .subscribe({
                        activity?.title = it.filename
                        rootView.report_detail.text = Core.convertReport(it.report, view)
             }))
        }

        return rootView
    }

    override fun onCreateOptionsMenu(menu: Menu, inflater: MenuInflater) {
        inflater.inflate(R.menu.menu_detail, menu)

        menu.findItem(R.id.action_export_report).let {
            it.setOnMenuItemClickListener {
                id?.let {
                    disposable.add(activityListener.getReportViewModel().getReport(it)
                            .subscribeOn(Schedulers.io())
                            .observeOn(AndroidSchedulers.mainThread())
                            .subscribe({
                                val intent: Intent = Intent(activity, ExportActivity::class.java)
                                intent.putExtra(ExportActivity.ARG_REPORT_NAME, it.filename)
                                intent.putExtra(ExportActivity.ARG_REPORT_DATA, it.report)
                                startActivity(intent)
                            }))
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
}
