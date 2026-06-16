#include <wx/wx.h>
#include "frames/MainFrame.h"

// ========== ПРИЛОЖЕНИЕ ==========
class MyApp : public wxApp {
public:
    virtual bool OnInit() override {
        // Установка обработчика исключений (опционально)
        //wxHandleFatalExceptions(true);
        
        MainFrame* mainFrame = new MainFrame("VisualCAL - Function Calculator");
        mainFrame->Show(true);
        
        return true;
    }
    
    virtual void OnFatalException() override {
        wxMessageBox("A fatal exception occurred!\nThe application will exit.",
                    "Fatal Error", wxOK | wxICON_ERROR);
    }
};

wxIMPLEMENT_APP(MyApp);