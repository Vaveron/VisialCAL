#include "FunctionLayer.h"

FunctionLayer::FunctionLayer(const wxString& name, std::function<double(double)> f)
    : mpFX(name), func(f), funcName(name) {
    SetContinuity(true);
    SetPen(wxPen(*wxRED, 4, wxPENSTYLE_SOLID));
}

double FunctionLayer::GetY(double x) {
    return func(x);
}

void FunctionLayer::SetFunction(std::function<double(double)> f) {
    func = f;
}

void FunctionLayer::SetLineColor(const wxColour& color, int width) {
    SetPen(wxPen(color, width, wxPENSTYLE_SOLID));
}