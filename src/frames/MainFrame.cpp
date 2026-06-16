#include "MainFrame.h"
#include "../layers/FunctionLayer.h"
#include "../utils/ExpressionParser.h"
#include "mathplot.h"
#include <wx/msgdlg.h>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_FIT_PLOT, MainFrame::OnFitPlot)
    EVT_MENU(ID_RESET_VIEW, MainFrame::OnResetView)
    EVT_MENU(wxID_EXIT, MainFrame::OnResetView)  // Временно, потом заменим
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title), m_textCtrl(nullptr), m_plot(nullptr), m_currentFunction(nullptr) {
    
    SetSize(900, 700);
    Centre();
    CreateStatusBar(2);
    // Главная панель
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Верхняя панель с элементами управления
    wxPanel* controlPanel = new wxPanel(panel);
    wxBoxSizer* controlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // Текстовое поле для ввода выражения
    wxStaticText* label = new wxStaticText(controlPanel, wxID_ANY, "f(x) = ");
    m_textCtrl = new wxTextCtrl(controlPanel, wxTextCtrl_id, "", 
                                wxDefaultPosition, wxSize(300, -1), wxTE_PROCESS_ENTER);
    
    // Кнопки управления
    wxButton* updateBtn = new wxButton(controlPanel, ID_UPDATE_FUNCTION, "Update");
    wxButton* fitBtn = new wxButton(controlPanel, ID_FIT_PLOT, "Fit");
    wxButton* resetBtn = new wxButton(controlPanel, ID_RESET_VIEW, "Reset");
    
    controlSizer->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    controlSizer->Add(m_textCtrl, 1, wxALL | wxEXPAND, 5);
    controlSizer->Add(updateBtn, 0, wxALL, 5);
    controlSizer->Add(fitBtn, 0, wxALL, 5);
    controlSizer->Add(resetBtn, 0, wxALL, 5);
    
    controlPanel->SetSizer(controlSizer);
    
    // Область графика
    m_plot = new mpWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
    m_plot->SetMinSize(wxSize(400, 400));
    
    mainSizer->Add(controlPanel, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(m_plot, 1, wxEXPAND | wxALL, 5);
    panel->SetSizer(mainSizer);
    //Настройки графика
    SetupPlot();
    // Создание меню и статус-бара
    SetStatusText("Ready. Enter expression and press Enter or click Update");
    
    // Привязка событий
    m_textCtrl->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnTextEnter, this);
    updateBtn->Bind(wxEVT_BUTTON, &MainFrame::OnTextEnter, this);
    fitBtn->Bind(wxEVT_BUTTON, &MainFrame::OnFitPlot, this);
    resetBtn->Bind(wxEVT_BUTTON, &MainFrame::OnResetView, this);
    
}

MainFrame::~MainFrame() {
    // Очистка
    if (m_currentFunction) {
        m_plot->DelLayer(m_currentFunction, false);
        delete m_currentFunction;
    }
}

void MainFrame::SetupPlot() {
    // Добавляем оси
    m_plot->AddLayer(new mpScaleX(wxT("X"), mpALIGN_CENTER, true));
    m_plot->AddLayer(new mpScaleY(wxT("Y"), mpALIGN_CENTER, true));
    
    // Добавляем сетку (опционально)
    m_plot->AddLayer(new mpInfoLegend(wxRect(0, 0, 0, 0), wxTRANSPARENT_BRUSH));
        // Создаём начальную функцию
    m_currentFunction = new FunctionLayer("", [](double x) {
        return 0;
    });
    
    // Устанавливаем цвет линии
    m_currentFunction->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    
    // Добавляем на график
    m_plot->AddLayer(m_currentFunction);
    // Настройки графика
    m_plot->EnableDoubleBuffer(true);
    m_plot->Fit();
}


void MainFrame::UpdateFunction(const wxString& expression) {
    std::string expr = expression.ToStdString();
    
    if (expr.empty()) {
        SetStatusText("Error: Empty expression", 1);
        return;
    }
    
    // Валидация выражения
    std::string errorMsg;
    if (!ExpressionParser::Validate(expr, errorMsg)) {
        SetStatusText("Parse error: " + wxString(expr) + " - " + wxString(errorMsg), 1);
        return;
    }
    
    // Обновляем функцию
    m_currentFunction->SetFunction(ExpressionParser::Parse(expr));
    
    // Обновляем статус
    SetStatusText("Function updated: f(x) = " + expression, 1);
    
    // Обновляем график
    m_plot->Fit();
    m_plot->Refresh();
}

void MainFrame::OnTextEnter(wxCommandEvent& event) {
    wxString text = m_textCtrl->GetValue();
    UpdateFunction(text);
}

void MainFrame::OnFitPlot(wxCommandEvent& event) {
    m_plot->Fit();
    m_plot->Refresh();
    SetStatusText("Plot fitted to window", 1);
}

void MainFrame::OnResetView(wxCommandEvent& event) {
    // Сброс масштаба и позиции
    // m_plot->SetMPScaleLim(0.01, 1000.0); // Этой строки нет в mathplot
    m_plot->SetPos(0, 0);                    // Сброс позиции
    m_plot->Fit();                           // Авто-масштабирование
    m_plot->Refresh();                       // Перерисовка
    SetStatusText("View reset", 1);
}