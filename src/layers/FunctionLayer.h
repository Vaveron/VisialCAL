#ifndef FUNCTION_LAYER_H
#define FUNCTION_LAYER_H

#include "mathplot.h"
#include <functional>
#include <wx/wx.h>

class FunctionLayer : public mpFX {
private:
    std::function<double(double)> func;
    wxString funcName;
    
public:
    // Конструктор принимает имя и функцию
    FunctionLayer(const wxString& name, std::function<double(double)> f);
    
    // Переопределяем виртуальную функцию
    virtual double GetY(double x) wxOVERRIDE;
    
    // Метод для смены функции на лету
    void SetFunction(std::function<double(double)> f);
    
    // Установка цвета линии
    void SetLineColor(const wxColour& color, int width = 2);
};

#endif // FUNCTION_LAYER_H