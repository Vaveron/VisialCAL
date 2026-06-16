#include <wx/wx.h>
#include "frames/MainFrame.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MainFrame *frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);