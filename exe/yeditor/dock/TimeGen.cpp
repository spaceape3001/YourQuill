////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TimeGen.hpp"
#include <yq-toolbox/basic/DelayInit.hpp>
#include <yq-toolbox/basic/Random.hpp>

#include <QDate>
#include <QDateEdit>
#include <QFormLayout>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>

namespace {
    void    reg_dategen()
    {
        register_dock<TimeGenDock>("Time Generator");
    }
    YQ_INVOKE(reg_dategen();)
    
}

using namespace yq;

TimeGen::TimeGen(QWidget* parent)
    : QDialog(parent)
{

    setContentsMargins(0,0,0,0);

    QFormLayout*    lay = new QFormLayout;
    m_start     = new QTimeEdit;
    m_end       = new QTimeEdit;
    m_result    = new QLineEdit;
    m_result->setReadOnly(true);
        
    m_start->setDisplayFormat("hh:mm:ss.zzz");
    m_start->setTime(QTime(0,0,0,0));
    m_end->setDisplayFormat("hh:mm:ss.zzz");
    m_end->setTime(QTime(23,59,59,999));
        
    QPushButton *gen    = new QPushButton(tr("Generate"));
    connect(gen, &QPushButton::clicked, this, &TimeGen::generate);
    lay->addRow(tr("Start"), m_start);
    lay->addRow(tr("End"), m_end);
    lay->addRow(gen, m_result);
    setLayout(lay);
        
    setWindowIcon(QIcon(":/icon/dice.svg"));
    setWindowTitle(tr("Time Generator"));
}

TimeGen::~TimeGen()
{
}

void                TimeGen::generate()
{
    QTime   s       = m_start->time();
    QTime   e       = m_end->time();
    int     dt      = s.msecsTo(e);
            
    double  f       = random(0., 1.);
    int     ms      = (int) (f*dt); // there's a bug in the uniformInt
        
    QTime   sug     = s.addMSecs(ms);
    QString str     = sug.toString("hh:mm:ss.zzz");
    m_result->setText(str);
}

TimeGenDock::TimeGenDock(QWidget*parent)
{
    setWidget(new TimeGen);
}

TimeGenDock::~TimeGenDock()
{
}


#include "moc_TimeGen.cpp"
