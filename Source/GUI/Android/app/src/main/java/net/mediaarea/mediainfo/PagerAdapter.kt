/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.os.Bundle

import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentStatePagerAdapter

class PagerAdapter(fm: FragmentManager, private var reports: List<Report>) : FragmentStatePagerAdapter(fm, BEHAVIOR_RESUME_ONLY_CURRENT_FRAGMENT) {
    override fun getCount(): Int {
        return reports.count()
    }

    override fun getItem(position: Int): Fragment {
        return ReportDetailFragment().apply {
            arguments = Bundle().apply {
                putInt(Core.ARG_REPORT_ID, reports.elementAt(position).id)
            }
        }
    }

    override fun getItemPosition(`object`: Any): Int {
        return POSITION_NONE
    }
}
