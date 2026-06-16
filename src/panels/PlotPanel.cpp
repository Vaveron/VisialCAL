#include "PlotPanel.h"

PlotPanel::PlotPanel(wxWindow *parent) 
    : wxPanel(parent, wxID_ANY) 
{
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    plot = new mpWindow(this, wxID_ANY);
    sizer->Add(plot, 1, wxEXPAND);
    SetSizer(sizer);
}