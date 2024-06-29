//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------

#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("..\..\..\Source\GUI\VCL\GUI_Preferences_Language.cpp", Preferences_LanguageF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Preferences_Output.cpp", Preferences_OutputF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Preferences.cpp", PreferencesF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Preferences_Custom.cpp", Preferences_CustomF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Preferences_Sheet.cpp", Preferences_SheetF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Web.cpp", WebF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Main.cpp", MainF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_About.cpp", AboutF);
USEFORM("..\..\..\Source\GUI\VCL\GUI_Export.cpp", ExportF);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
        Application->Initialize();
        Application->MainFormOnTaskBar = true;
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
