/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.os.Bundle
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentActivity
import androidx.recyclerview.widget.DiffUtil
import androidx.viewpager2.adapter.FragmentStateAdapter

/**
 * Adapter responsible for managing the lifecycle and presentation of [ReportDetailFragment]
 * instances within a dynamic data collection stream.
 *
 * Generated with Google Gemini 3.5 Flash
 */
class PagerAdapter(fa: FragmentActivity) : FragmentStateAdapter(fa) {

    private var reports: List<Report> = emptyList()

    /**
     * Required for stable fragment reuse and structural mutation tracking.
     */
    override fun getItemId(position: Int): Long {
        return reports[position].id.toLong()
    }

    /**
     * Informs ViewPager2 whether a specific fragment identity still exists in the dataset.
     */
    override fun containsItem(itemId: Long): Boolean {
        return reports.any { it.id.toLong() == itemId }
    }

    /**
     * Calculates the structural differences between the current and incoming datasets
     * using an optimized metadata-only comparison strategy.
     */
    fun submitList(newReports: List<Report>) {
        val oldReports = this.reports

        val diffResult = DiffUtil.calculateDiff(object : DiffUtil.Callback() {
            override fun getOldListSize(): Int = oldReports.size
            override fun getNewListSize(): Int = newReports.size

            override fun areItemsTheSame(oldItemPosition: Int, newItemPosition: Int): Boolean {
                return oldReports[oldItemPosition].id == newReports[newItemPosition].id
            }

            override fun areContentsTheSame(oldItemPosition: Int, newItemPosition: Int): Boolean {
                val oldItem = oldReports[oldItemPosition]
                val newItem = newReports[newItemPosition]

                // Intentionally bypasses binary blob (ByteArray) comparison to avoid main-thread performance degradation.
                // Evaluation is strictly restricted to structural presentation properties.
                return oldItem.id == newItem.id &&
                        oldItem.filename == newItem.filename &&
                        oldItem.version == newItem.version
            }
        })

        this.reports = newReports
        diffResult.dispatchUpdatesTo(this)
    }

    override fun createFragment(position: Int): Fragment {
        return ReportDetailFragment().apply {
            arguments = Bundle().apply {
                putInt(Core.ARG_REPORT_ID, reports[position].id)
            }
        }
    }

    override fun getItemCount(): Int {
        return reports.size
    }
}
