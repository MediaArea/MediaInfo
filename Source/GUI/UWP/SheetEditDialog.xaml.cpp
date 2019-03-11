/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include "pch.h"
#include "SheetEditDialog.xaml.h"

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
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;

//---------------------------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
SheetEditDialog::SheetEditDialog(Platform::String^ Title, Sheet^ Content) : _Sheet(Content)
{
    InitializeComponent();

    this->Title=Title;

    SolidColorBrush^ TransparentBrush=ref new SolidColorBrush(Windows::UI::Colors::White);
    TransparentBrush->Opacity=0;

    SolidColorBrush^ GrayBrush=ref new SolidColorBrush(Windows::UI::Colors::Gray);

    NameText->Text=Content->Name;
    for (Field^ It:Content->Fields)
    {
        Add_Field(It->Title, It->Length, It->StreamKind, It->StreamPos, It->Parameter);
    }

    FontIcon^ AddIcon=ref new FontIcon();
    AddIcon->FontFamily=ref new Windows::UI::Xaml::Media::FontFamily(L"Segoe MDL2 Assets");
    AddIcon->Glyph=L"\uE710";

    Button^ AddButton=ref new Button();
    AddButton->Background=TransparentBrush;
    AddButton->Foreground=GrayBrush;
    AddButton->HorizontalAlignment=Windows::UI::Xaml::HorizontalAlignment::Stretch;
    AddButton->Content=AddIcon;

    FieldList->Footer=AddButton;

    AddButton->Click+=ref new RoutedEventHandler([this](Object^, RoutedEventArgs^) {
        Add_Field(L"New Field", 1, 0, 0, L"FileName");
    });
}

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void SheetEditDialog::Add_Field(Platform::String^ Name, size_t Width, size_t StreamKind, size_t StreamPos, Platform::String^ Parameter)
{
    SolidColorBrush^ TransparentBrush=ref new SolidColorBrush(Windows::UI::Colors::White);
    TransparentBrush->Opacity=0;

    SolidColorBrush^ GrayBrush=ref new SolidColorBrush(Windows::UI::Colors::Gray);

    RelativePanel^ FieldPanel=ref new RelativePanel();
    FieldPanel->Margin=ThicknessHelper::FromLengths(0, 5, 0, 5);

    FontIcon^ DeleteIcon=ref new FontIcon();
    DeleteIcon->FontFamily=ref new Windows::UI::Xaml::Media::FontFamily(L"Segoe MDL2 Assets");
    DeleteIcon->FontSize=18;
    DeleteIcon->Glyph=L"\uE711";

    Button^ DeleteButton=ref new Button();
    DeleteButton->Background=TransparentBrush;
    DeleteButton->Foreground=GrayBrush;
    DeleteButton->VerticalAlignment=Windows::UI::Xaml::VerticalAlignment::Stretch;
    DeleteButton->Content=DeleteIcon;
    FieldPanel->Children->Append(DeleteButton);
    FieldPanel->SetAlignRightWithPanel(DeleteButton, true);
    FieldPanel->SetAlignTopWithPanel(DeleteButton, true);
    FieldPanel->SetAlignBottomWithPanel(DeleteButton, true);

    SheetEditFieldControl^ FieldControl=ref new SheetEditFieldControl(Name, Width, StreamKind, StreamPos, Parameter);
    FieldPanel->Children->Append(FieldControl);
    FieldPanel->SetLeftOf(FieldControl, DeleteButton);
    FieldPanel->SetAlignLeftWithPanel(FieldControl, true);
    FieldPanel->SetAlignTopWithPanel(FieldControl, true);
    FieldPanel->SetAlignBottomWithPanel(FieldControl, true);

    FieldList->Items->Append(FieldPanel);

    DeleteButton->Click+=ref new RoutedEventHandler([this](Object^ Sender, RoutedEventArgs^ Event) {
        Button^ Item=safe_cast<Button^>(Sender);

        unsigned int Index=0;
        if (FieldList->Items->IndexOf(Item->Parent, &Index))
            FieldList->Items->RemoveAt(Index);
    });
}

//---------------------------------------------------------------------------
void SheetEditDialog::ContentDialog_PrimaryButtonClick(ContentDialog^ Sender, ContentDialogButtonClickEventArgs^ Event)
{
    if (NameText->Text->Length())
        _Sheet->Name=NameText->Text;

    Vector<Field^>^ Fields=ref new Vector<Field^>();
    for (Object^ Obj : FieldList->Items)
    {
        if (Obj->GetType()->FullName!=TypeName(RelativePanel::typeid).Name)
            continue;

        RelativePanel^ Panel=safe_cast<RelativePanel^>(Obj);

        SheetEditFieldControl^ FieldControl=nullptr;
        for (Object^ Obj2 : Panel->Children)
        {
            if (Obj2->GetType()->FullName!=TypeName(SheetEditFieldControl::typeid).Name)
                continue;

            FieldControl=safe_cast<SheetEditFieldControl^>(Obj2);
        }

        if (!FieldControl)
            continue;

        Fields->Append(ref new Field(FieldControl->Get_Name(), FieldControl->Get_StreamKind(), FieldControl->Get_StreamPos(), FieldControl->Get_Parameter(), FieldControl->Get_Width()));
    }

    _Sheet->Fields=Fields;
}

//---------------------------------------------------------------------------
void SheetEditDialog::ContentDialog_SecondaryButtonClick(ContentDialog^ Sender, ContentDialogButtonClickEventArgs^ Event)
{
}
