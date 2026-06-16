#include "MainFrame.h"
#include "../panels/PlotPanel.h"
#include "../math/Calculator.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
wxEND_EVENT_TABLE()

MainFrame::MainFrame() 
    : wxFrame(nullptr, wxID_ANY, "VisualCAL", 
              wxDefaultPosition, wxSize(800, 600)) 
{
    // Создаём панель с графиком
    PlotPanel *plotPanel = new PlotPanel(this);
    
    // Создаём меню
    wxMenuBar *menuBar = new wxMenuBar();
    wxMenu *fileMenu = new wxMenu();
    fileMenu->Append(wxID_EXIT);
    menuBar->Append(fileMenu, "&Файл");
    SetMenuBar(menuBar);
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}