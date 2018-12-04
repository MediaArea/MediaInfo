/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import androidx.core.app.NavUtils
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProviders

import android.content.Intent
import android.os.Bundle
import android.view.MenuItem

import kotlinx.android.synthetic.main.activity_report_detail.*

class ReportDetailActivity : AppCompatActivity(), ReportActivityListener {
    private lateinit var reportModel: ReportViewModel

    override fun getReportViewModel(): ReportViewModel {
        return reportModel
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_report_detail)
        setSupportActionBar(detail_toolbar)

        // Show the Up button in the action bar.
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        val viewModelFactory = Injection.provideViewModelFactory(this)
        ViewModelProviders.of(this, viewModelFactory).get(ReportViewModel::class.java)
        reportModel = ViewModelProviders.of(this, viewModelFactory).get(ReportViewModel::class.java)

        // savedInstanceState is non-null when there is fragment state
        // saved from previous configurations of this activity
        // (e.g. when rotating the screen from portrait to landscape).
        // In this case, the fragment will automatically be re-added
        // to its container so we don't need to manually add it.
        // For more information, see the Fragments API guide at:
        //
        // http://developer.android.com/guide/components/fragments.html
        //
        if (savedInstanceState == null) {
            // Create the detail fragment and add it to the activity
            // using a fragment transaction.
            val fragment = ReportDetailFragment().apply {
                arguments = Bundle().apply {
                    putInt(ReportDetailFragment.ARG_REPORT_ID,
                            intent.getIntExtra(ReportDetailFragment.ARG_REPORT_ID, -1))
                }
            }

            supportFragmentManager.beginTransaction()
                    .add(R.id.report_detail_container, fragment)
                    .commit()
        }
    }

    override fun onOptionsItemSelected(item: MenuItem) = when (item.itemId) {
        android.R.id.home -> {
            NavUtils.navigateUpTo(this, Intent(this, ReportListActivity::class.java))
            true

        }
        else -> super.onOptionsItemSelected(item)
    }
}
