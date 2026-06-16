#pragma once
#include <wx/wx.h>
#include <wx/frame.h>

class MainFrame : public wxFrame {
public:
    MainFrame();
    
private:
    void OnPlot(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    
    wxDECLARE_EVENT_TABLE();
};