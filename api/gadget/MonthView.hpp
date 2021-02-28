////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <QDialog>

class QCalendarWidget;
class QSpinBox;
class QComboBox;

class  MonthView : public QDialog {
    Q_OBJECT
public:
    MonthView(QWidget*parent=nullptr);
    ~MonthView();

private slots:
    void                userChange();
    void                calendarChange();
    
private:
    QSpinBox*           m_year;
    QComboBox*          m_month;
    QCalendarWidget*    m_calendar;

};
