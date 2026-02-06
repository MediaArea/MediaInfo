/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProvider

import android.content.Intent
import android.os.Bundle
import android.os.Build

import android.view.MenuItem

import android.content.res.AssetManager
import android.view.View
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.updatePadding

import androidx.viewpager.widget.ViewPager

import io.reactivex.disposables.CompositeDisposable
import io.reactivex.schedulers.Schedulers
import io.reactivex.android.schedulers.AndroidSchedulers
import net.mediaarea.mediainfo.databinding.ActivityReportDetailBinding

class ReportDetailActivity : AppCompatActivity(), ReportActivityListener {
    private lateinit var activityReportDetailBinding: ActivityReportDetailBinding

    private inner class PageChangeListener(private val reports: List<Report>) : ViewPager.SimpleOnPageChangeListener() {
        override fun onPageSelected(position: Int) {
            super.onPageSelected(position)
            title = reports.elementAt(position).filename
            intent.putExtra(Core.ARG_REPORT_ID, reports.elementAt(position).id)

        }
    }
    private val disposable: CompositeDisposable = CompositeDisposable()
    private lateinit var reportModel: ReportViewModel

    override fun getReportViewModel(): ReportViewModel {
        return reportModel
    }

    // BUG: appcompat-1.1.0 returns resource that breaks old WebView implementations
    override fun getAssets(): AssetManager {
        if (Build.VERSION.SDK_INT in 21..25) {
            return resources.assets
        }
        return super.getAssets()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        activityReportDetailBinding = ActivityReportDetailBinding.inflate(layoutInflater)
        setContentView(activityReportDetailBinding.root)

        ViewCompat.setOnApplyWindowInsetsListener(activityReportDetailBinding.appBar) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updatePadding(
                left = bars.left,
                top = bars.top,
                right = bars.right,
            )
            WindowInsetsCompat.CONSUMED
        }
        ViewCompat.setOnApplyWindowInsetsListener(activityReportDetailBinding.pager) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updatePadding(
                bottom = bars.bottom,
            )
            WindowInsetsCompat.CONSUMED
        }

        setSupportActionBar(activityReportDetailBinding.detailToolbar)

        // Show the Up button in the action bar.
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        if (resources.getBoolean(R.bool.has_two_pane)) {
            finish()
            return
        }

        val viewModelFactory = Injection.provideViewModelFactory(this)
        reportModel = ViewModelProvider(this, viewModelFactory)[ReportViewModel::class.java]

        disposable.add(reportModel.getAllReports()
            .subscribeOn(Schedulers.io())
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe { reports: List<Report> ->
                activityReportDetailBinding.pager.addOnPageChangeListener(PageChangeListener(reports))
                activityReportDetailBinding.pager.adapter = PagerAdapter(supportFragmentManager, reports)
                val id = intent.getIntExtra(Core.ARG_REPORT_ID, -1)
                if (id!=-1) {
                    val index = reports.indexOfFirst { it.id == id }
                    if (index!=-1) {
                        title = reports.elementAt(index).filename
                        activityReportDetailBinding.pager.setCurrentItem(index, false)
                    }
                }
            })
    }

    override fun onStop() {
        super.onStop()

        // clear all the subscription
        disposable.clear()
    }

    override fun finish() {
        val id = intent.getIntExtra(Core.ARG_REPORT_ID, -1)
        val result = Intent()
        result.putExtra(Core.ARG_REPORT_ID, id)
        setResult(RESULT_OK, result)

        super.finish()
    }

    override fun onOptionsItemSelected(item: MenuItem) = when (item.itemId) {
        android.R.id.home -> {
            finish()
            true
        }
        else -> super.onOptionsItemSelected(item)
    }
}
