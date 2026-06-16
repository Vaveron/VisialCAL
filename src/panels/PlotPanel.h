#pragma once
#include <wx/wx.h>
#include <wx/panel.h>
#include "mathplot.h"  // Ваша библиотека

class PlotPanel : public wxPanel {
public:
    PlotPanel(wxWindow *parent);
    
private:
    mpWindow *plot;
};