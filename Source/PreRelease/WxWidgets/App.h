#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

class App : public wxApp
{
public:
    virtual bool OnInit();
};

IMPLEMENT_APP(App)
