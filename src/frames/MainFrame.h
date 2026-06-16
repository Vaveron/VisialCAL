#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/spinctrl.h>

// Forward declarations
class mpWindow;
class FunctionLayer;

class MainFrame : public wxFrame {
public:
    enum IDs {
        wxTextCtrl_id = 6,
        ID_UPDATE_FUNCTION = 7,
        ID_FIT_PLOT = 8,
        ID_RESET_VIEW = 9
    };
    
    MainFrame(const wxString& title);
    virtual ~MainFrame();
    
    // Обновление графика по новому выражению
    void UpdateFunction(const wxString& expression);
    
    // Обработчики событий
    void OnTextEnter(wxCommandEvent& event);
    void OnFitPlot(wxCommandEvent& event);
    void OnResetView(wxCommandEvent& event);
    
private:
    void CreateToolBar();
    void SetupPlot();
    
    wxTextCtrl* m_textCtrl;
    mpWindow* m_plot;
    FunctionLayer* m_currentFunction;
    
    wxDECLARE_EVENT_TABLE();
};

#endif // MAIN_FRAME_H