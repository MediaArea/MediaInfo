/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.os.Bundle

import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentActivity
import androidx.viewpager2.adapter.FragmentStateAdapter

class PagerAdapter(fa: FragmentActivity, private var reports: List<Report>) : FragmentStateAdapter(fa) {

    override fun createFragment(position: Int): Fragment {
        return ReportDetailFragment().apply {
            arguments = Bundle().apply {
                putInt(Core.ARG_REPORT_ID, reports.elementAt(position).id)
            }
        }
    }

    override fun getItemCount(): Int {
        return reports.count()
    }

}