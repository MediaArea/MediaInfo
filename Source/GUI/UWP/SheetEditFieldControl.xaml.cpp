/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include <wchar.h>

#include "pch.h"
#include "SheetEditFieldControl.xaml.h"

#include "ZenLib/Ztring.h"
#include "ZenLib/ZtringList.h"
#include "ZenLib/ZtringListList.h"
#include "MediaInfo/MediaInfo_Internal.h"

//---------------------------------------------------------------------------
using namespace ZenLib;
using namespace MediaInfo;
using namespace MediaInfoLib;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

//---------------------------------------------------------------------------
Map<size_t, Platform::String^>^ SheetEditFieldControl::_Streams=nullptr;
Map<size_t, IMapView<Platform::String^, Platform::String^>^>^ SheetEditFieldControl::_Parameters=nullptr;

//---------------------------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
SheetEditFieldControl::SheetEditFieldControl(Platform::String^ Name, size_t Width, size_t StreamKind, size_t StreamPos, Platform::String^ Parameter)
{
    InitializeComponent();

    if (!_Streams)
        Get_Streams();
    if (!_Parameters)
        Get_Parameters();

    NameText->Text=Name;
    SizeBox->SelectedIndex=Width-1<9?Width-1:1;

    PosText->Text=ref new Platform::String(Ztring().From_Number(StreamPos).c_str());

    for (IKeyValuePair<size_t, Platform::String^>^ Stream : _Streams)
    {
        ComboBoxItem^ Item=ref new ComboBoxItem();
        Item->Content=Stream->Value;
        Item->Tag=Stream->Key;

        StreamBox->Items->Append(Item);

        if (Stream->Key==StreamKind)
            StreamBox->SelectedItem=Item;
    }

   if (StreamBox->Items->Size && StreamBox->SelectedItem==nullptr)
        StreamBox->SelectedIndex=0;

    Object^ Item=ParameterBox->FindName(Parameter);
    if (Item)
        ParameterBox->SelectedItem=Item;
}

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Platform::String^ SheetEditFieldControl::Get_Name()
{
    if (!NameText->Text)
        return Get_Parameter();

    return NameText->Text;
}

//---------------------------------------------------------------------------
size_t SheetEditFieldControl::Get_Width()
{
    return SizeBox->SelectedIndex+1;
}

//---------------------------------------------------------------------------
size_t SheetEditFieldControl::Get_StreamKind()
{
    if (!StreamBox->SelectedItem)
        return 0;

    FrameworkElement^ SelectedItem=safe_cast<FrameworkElement^>(StreamBox->SelectedItem);
    return static_cast<size_t>(SelectedItem->Tag);
}

//---------------------------------------------------------------------------
size_t SheetEditFieldControl::Get_StreamPos()
{
    return Ztring(PosText->Text->Data()).To_int32u();
}

//---------------------------------------------------------------------------
Platform::String^ SheetEditFieldControl::Get_Parameter()
{
    if (!ParameterBox->SelectedItem)
        return L"";

    FrameworkElement^ SelectedItem=safe_cast<FrameworkElement^>(ParameterBox->SelectedItem);
    return SelectedItem->Name;
}

//---------------------------------------------------------------------------
void SheetEditFieldControl::Get_Streams()
{
    _Streams=ref new Map<size_t, Platform::String^>();
    for (size_t StreamKind=static_cast<size_t>(Stream_General); StreamKind<static_cast<size_t>(Stream_Max); StreamKind++)
    {
        _Streams->Insert(StreamKind, AppCore::Get_Stream_Name(StreamKind));
    }
}

//---------------------------------------------------------------------------
void SheetEditFieldControl::Get_Parameters()
{
    _Parameters=ref new Map<size_t, IMapView<Platform::String^, Platform::String^>^>();

    //TODO: Handle this in MediaInfoLib for portability and resilience
    Ztring ParametersCSV=MediaInfo_Internal().Option(__T("Info_Parameters_CSV"));

    ZtringList ParametersByStream;
    ParametersByStream.Separator_Set(0, Ztring(ZenLib::EOL)+Ztring(ZenLib::EOL));
    ParametersByStream.Write(ParametersCSV);

    for (Ztring Pos : ParametersByStream)
    {
        ZtringListList Stream;
        Stream.Separator_Set(0, ZenLib::EOL);
        Stream.Separator_Set(1, __T(";"));

        Stream.Write(Pos);

        Ztring StreamKind=Stream.Read(0);
        Stream.Delete(0);

        if (StreamKind.empty())
            continue;

        Map<Platform::String^, Platform::String^>^ Parameters=ref new Map<Platform::String^, Platform::String^>();
        for (ZtringList Pos2 : Stream)
        {
            if (Pos2.size()<2 || Pos2.Read(0).empty())
                continue;

            Parameters->Insert(ref new Platform::String(Pos2.Read(0).c_str()), ref new Platform::String(Pos2.Read(1).c_str()));
        }

        _Parameters->Insert(AppCore::Get_Stream_Id(ref new Platform::String(StreamKind.c_str())), Parameters->GetView());
    }
}

//---------------------------------------------------------------------------
void SheetEditFieldControl::StreamBox_SelectionChanged(Object^ Sender, SelectionChangedEventArgs^ Event)
{
    ParameterBox->Items->Clear();

    if (!StreamBox->SelectedItem)
        return;

    ComboBoxItem^ SelectedItem=safe_cast<ComboBoxItem^>(StreamBox->SelectedItem);

    if (!_Parameters->HasKey(static_cast<size_t>(SelectedItem->Tag)))
        return;

    for (IKeyValuePair<Platform::String^, Platform::String^>^ Parameter : _Parameters->Lookup(static_cast<size_t>(SelectedItem->Tag)))
    {
        ComboBoxItem^ Item=ref new ComboBoxItem();
        Item->Content=Parameter->Key;
        Item->Name=Parameter->Key;
        Item->Tag=Parameter->Value;

        ParameterBox->Items->Append(Item);
    }

    if (ParameterBox->Items->Size)
        ParameterBox->SelectedIndex=0;
}

//---------------------------------------------------------------------------
void SheetEditFieldControl::ParameterBox_SelectionChanged(Object^ Sender, SelectionChangedEventArgs^ Event)
{
    if (ParameterBox->Items->Size && ParameterBox->SelectedItem)
    {
        ComboBoxItem^ Item=safe_cast<ComboBoxItem^>(ParameterBox->SelectedItem);
        DescText->Text=safe_cast<Platform::String^>(Item->Tag);
    }
}

//---------------------------------------------------------------------------
void SheetEditFieldControl::NumText_TextChanging(TextBox^ Sender, TextBoxTextChangingEventArgs^ Event)
{
    Platform::String^ Content=L"";
    for (wchar_t Char : Sender->Text)
    {
        if (iswdigit(Char))
            Content+=Char;
    }
    if (Content->Length()==0)
        Content=L"0";

    Sender->Text=Content;
}
