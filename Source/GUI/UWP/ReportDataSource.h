/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
namespace MediaInfo
{
    [Windows::UI::Xaml::Data::Bindable]
    public ref class ReportDataSource sealed
    {
    public:
        static Windows::Foundation::Collections::IObservableVector<Report^>^ GetAllReports()
        {
            return _Reports;
        }

        static Report^ GetReportById(uint64 Id)
        {
            for (Report^ It : _Reports)
            {
                if (It->Id==Id)
                    return It;
            }

            return nullptr;
        }

        static Report^ GetReportAt(size_t Pos)
        {
            return _Reports->GetAt(Pos);
        }

        static void AddReport(Report^ ToAdd)
        {
            _Reports->Append(ToAdd);
        }

        static void DeleteReport(Report^ ToDelete)
        {
            unsigned int Index=0;
            if (_Reports->IndexOf(ToDelete, &Index))
                _Reports->RemoveAt(Index);
        }

    private:
        ReportDataSource() {};

        static Windows::Foundation::Collections::IObservableVector<Report^>^ _Reports;
    };
}