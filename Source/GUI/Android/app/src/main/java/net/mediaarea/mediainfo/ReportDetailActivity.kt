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

import android.content.res.AssetManager
import android.view.MenuItem
import android.view.View
import androidx.core.view.ViewCompat
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.updatePadding
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.lifecycleScope
import androidx.lifecycle.repeatOnLifecycle
import androidx.viewpager2.widget.ViewPager2

import kotlinx.coroutines.flow.collectLatest
import kotlinx.coroutines.launch

import net.mediaarea.mediainfo.databinding.ActivityReportDetailBinding

class ReportDetailActivity : AppCompatActivity(), ReportActivityListener {
    private lateinit var activityReportDetailBinding: ActivityReportDetailBinding

    private inner class PageChangeListener(private val reports: List<Report>) : ViewPager2.OnPageChangeCallback() {
        override fun onPageSelected(position: Int) {
            super.onPageSelected(position)

            // CRITICAL BOUNDS CHECK: Verify position is valid inside the current data allocation range
            if (position >= 0 && position < reports.size) {
                title = reports.elementAt(position).filename
                intent.putExtra(Core.ARG_REPORT_ID, reports.elementAt(position).id)
            }
        }
    }
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

        WindowCompat.enableEdgeToEdge(window)
        ViewCompat.setOnApplyWindowInsetsListener(activityReportDetailBinding.appBar) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updatePadding(
                left = bars.left,
                top = bars.top,
                right = bars.right,
            )
            insets
        }
        ViewCompat.setOnApplyWindowInsetsListener(activityReportDetailBinding.pager) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updatePadding(
                left = bars.left,
                right = bars.right,
                bottom = bars.bottom,
            )
            insets
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

        /**
         * Establishes a reactive, lifecycle-aware database stream to manage the ViewPager2 presentation.
         *
         * This block handles three primary architectural responsibilities:
         * 1. Lifecycle Safety: Collects database states only when the UI is visible, automatically
         *    pausing collections during background states to optimize resource consumption.
         * 2. State & Memory Management: Dynamically refreshes the UI page structural components, updating
         *    the metadata adapters and cycling out listener references to eliminate memory leaks.
         * 3. Deep-Link Navigation: Synchronizes initial position routing via incoming Intents exactly
         *    once on screen establishment, while preserving user viewport positioning during background/foreground flips.
         *
         *    From Google Gemini 3.5 Flash
         */
        val pagerAdapter = PagerAdapter(this)
        activityReportDetailBinding.pager.adapter = pagerAdapter
        var pageChangeListener: PageChangeListener? = null
        lifecycleScope.launch {
            // Structural flag to isolate one-time deep-link positioning from data streams
            var isInitialSetupDone = false
            repeatOnLifecycle(Lifecycle.State.STARTED) {
                reportModel.getAllReports().collectLatest { reports: List<Report> ->
                    // Clean up previous listener instances to prevent memory leaks and stacked callbacks
                    pageChangeListener?.let { oldListener ->
                        activityReportDetailBinding.pager.unregisterOnPageChangeCallback(oldListener)
                    }
                    // Short-circuit to clean UI state if database table is completely empty
                    if (reports.isEmpty()) {
                        title = getString(R.string.app_name)
                        pagerAdapter.submitList(emptyList())
                        pageChangeListener = null
                        return@collectLatest
                    }
                    // Re-instantiate tracking callback using verified current data parameters
                    pageChangeListener = PageChangeListener(reports)
                    activityReportDetailBinding.pager.registerOnPageChangeCallback(pageChangeListener!!)
                    pagerAdapter.submitList(reports)
                    // Handle initial entry positioning exactly once per Activity session
                    if (!isInitialSetupDone) {
                        val targetId = intent.getIntExtra(Core.ARG_REPORT_ID, -1)
                        if (targetId != -1) {
                            val index = reports.indexOfFirst { it.id == targetId }
                            if (index != -1) {
                                title = reports[index].filename
                                activityReportDetailBinding.pager.setCurrentItem(index, false)
                            }
                        }
                        isInitialSetupDone = true
                    } else {
                        // Dynamically sync header title with structural updates during ongoing collection emissions
                        val currentIndex = activityReportDetailBinding.pager.currentItem
                        if (currentIndex in reports.indices) {
                            title = reports[currentIndex].filename
                        }
                    }
                }
            }
        }
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
