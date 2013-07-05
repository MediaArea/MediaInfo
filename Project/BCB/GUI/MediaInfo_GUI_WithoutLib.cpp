//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("..\..\..\Source\GUI\VCL\GUI_Preferences_Custom.cpp", Preferences_CustomF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Export.cpp", ExportF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Preferences_Output.cpp", Preferences_OutputF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Preferences_Language.cpp", Preferences_LanguageF); /* TTntForm: File Type */
USEFORM("..\..\..\Source\GUI\VCL\GUI_Web.cpp", WebF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Main.cpp", MainF); /* TTntForm: File Type */
USEFORM("..\..\..\Source\GUI\VCL\GUI_About.cpp", AboutF); /* TTntForm: File Type */
USEFORM("..\..\..\Source\GUI\VCL\GUI_Preferences.cpp", PreferencesF); /* TTntForm: File Type */
USEFORM("..\..\..\Source\GUI\VCL\GUI_Preferences_Sheet.cpp", Preferences_SheetF);
//---------------------------------------------------------------------------
WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    try
    {
        Application->Initialize();
        Application->Title = "MediaInfo";
        Application->CreateForm(__classid(TMainF), &MainF);
         Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
