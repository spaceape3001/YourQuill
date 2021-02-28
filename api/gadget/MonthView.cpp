////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "MonthView.hpp"

#include <QCalendar>
#include <QCalendarWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

MonthView::MonthView(QWidget*parent) : 
    QDialog(parent), 
    m_year(nullptr), 
    m_month(nullptr),
    m_calendar(nullptr)
{
    QCalendar   cal;
    setContentsMargins(0,0,0,0);
    m_year      = new QSpinBox;
    m_year->setRange(1,9999);
    m_month     = new QComboBox;
    for(int i=1;i<=12;++i){
        m_month->addItem(cal.monthName(QLocale(), i), i);
    }
    m_calendar   = new QCalendarWidget;
    m_year->setValue(m_calendar->yearShown());
    m_month->setCurrentIndex(m_calendar->monthShown() - 1);

    QHBoxLayout*        hlay    = new QHBoxLayout;
    hlay->addWidget(m_year,0);
    hlay->addWidget(m_month, 0);
    hlay->addStretch(1);
    hlay->    setContentsMargins(0,0,0,0);

    QVBoxLayout*        vlay        = new QVBoxLayout;
    vlay->addLayout(hlay, 0);
    vlay->addWidget(m_calendar,0);
    vlay->addStretch(1);
    //vlay->    setContentsMargins(0,0,0,0);
    setLayout(vlay);

    connect(m_calendar, SIGNAL(currentPageChanged(int,int)), this, SLOT(calendarChange()));
    connect(m_year, SIGNAL(valueChanged(int)), this, SLOT(userChange()));
    connect(m_month, SIGNAL(currentIndexChanged(int)), this, SLOT(userChange()));

    setWindowTitle(tr("Calendar"));
}

MonthView::~MonthView()
{
}

void                MonthView::userChange()
{
    int     year        = m_year->value();
    int     month       = m_month->currentIndex() + 1;
    m_calendar->setCurrentPage(year, month);
}

void                MonthView::calendarChange()
{
    int     year        = m_calendar->yearShown();
    int     month       = m_calendar->monthShown();
    m_year->setValue(year);
    m_month->setCurrentIndex(month - 1);
}

#include "moc_MonthView.cpp"
