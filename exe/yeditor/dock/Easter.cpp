////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Easter.hpp"
#include <QSpinBox>
#include <QDateEdit>
#include <QDate>
#include <QHBoxLayout>
#include <yq/date/Date.hpp>
#include <yq/core/DelayInit.hpp>

using namespace yq;

namespace {
    void    reg_easter()
    {
        register_dock<EasterDock>("Easter Calculator");
    }
    YQ_INVOKE(reg_easter();)
    
}

Easter::Easter(QWidget*parent) : QDialog(parent)
{
    setContentsMargins(0,0,0,0);
    QHBoxLayout*    lay = new QHBoxLayout;
    
    m_year      = new QSpinBox;
    m_year->setRange(0,10000);
    lay->addWidget(m_year);
    m_date  = new QDateEdit;
    m_date->setEnabled(false);
    lay->addWidget(m_date);
    setLayout(lay);
    
    setWindowTitle(tr("Easter Calculator"));
    
    QDate today = QDate::currentDate();
    m_year->setValue(today.year());
    yearChange();
    
    connect(m_year, &QSpinBox::valueChanged, this, &Easter::yearChange);
}

Easter::~Easter()
{
}

void        Easter::yearChange()
{
    int     y   = m_year -> value();
    Date    ed(EASTER, (int16_t) y);
    m_date->setDate(QDate(ed.year, ed.month, ed.day));
}


EasterDock::EasterDock(QWidget*parent)
{
    setWidget(new Easter);
}

EasterDock::~EasterDock()
{
}


#include "moc_Easter.cpp"

