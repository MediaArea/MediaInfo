/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import java.io.OutputStream

import android.support.v7.app.AppCompatActivity
import android.support.v4.provider.DocumentFile
import android.support.design.widget.Snackbar
import android.os.Bundle
import android.app.Activity
import android.content.SharedPreferences
import android.content.Context
import android.content.Intent
import android.widget.ArrayAdapter

import kotlinx.android.synthetic.main.activity_export.*

class ExportActivity : AppCompatActivity() {
    companion object {
        const val SAVE_FILE_REQUEST_CODE: Int = 1
        const val ARG_REPORT_NAME: String = "name"
        const val ARG_REPORT_DATA: String = "report"
    }

    private lateinit var sharedPreferences: SharedPreferences
    private var report: ByteArray = ByteArray(0)
    private var name: String = ""

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_export)

        sharedPreferences = getSharedPreferences(getString(R.string.preferences_key), Context.MODE_PRIVATE)

        var view: String = sharedPreferences.getString(getString(R.string.preferences_view_key), "Text")

        val adapter: ArrayAdapter<Core.ReportView> = ArrayAdapter(this,
                android.R.layout.simple_spinner_dropdown_item, mutableListOf())

        export_view_spinner.adapter = adapter

        Core.views.forEach {
            adapter.add(it)

            if (it.name == view)
                export_view_spinner.setSelection(adapter.count - 1)
        }

        report = intent.getByteArrayExtra(ARG_REPORT_DATA)
        name = intent.getStringExtra(ARG_REPORT_NAME)

        close_btn.setOnClickListener {
            finish()
        }

        export_btn.setOnClickListener {
            val intent: Intent = Intent(Intent.ACTION_OPEN_DOCUMENT_TREE)
            startActivityForResult(intent, SAVE_FILE_REQUEST_CODE)
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, resultData: Intent?) {
        if (resultCode == Activity.RESULT_OK) {
            when (requestCode) {
                SAVE_FILE_REQUEST_CODE -> {
                    if (resultData == null) {
                        onError()
                        return
                    }


                    if(report.isEmpty()) {
                        onError()
                        return
                    }

                    val directory: DocumentFile = DocumentFile.fromTreeUri(this, resultData.data)

                    if (!directory.canWrite())
                        onError()

                    val view: Core.ReportView = export_view_spinner.selectedItem as Core.ReportView
                    val reportText: String = Core.convertReport(report, view.name, true)
                    val filename: String =  String.format("%s.%s", name, view.name)

                    try {
                        val document: DocumentFile = directory.createFile(view.mime, filename)
                        val ostream: OutputStream = contentResolver.openOutputStream(document.uri)

                        ostream.write(reportText.toByteArray())
                        ostream.flush()
                        ostream.close()
                    } catch (e: Exception) {
                        onError()
                        return
                    }

                    finish()
                }
            }
        }
    }

    private fun onError() {
        // Show error message for 5 seconds
        Snackbar.make(export_layout.rootView, R.string.error_text, 5000)
                .setAction("Action", null)
                .show()
    }
}