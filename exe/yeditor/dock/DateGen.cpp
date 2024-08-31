////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DateGen.hpp"

#include <yq/basic/DelayInit.hpp>
#include <yq/basic/Random.hpp>

#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>

namespace {
    void    reg_dategen()
    {
        register_dock<DateGenDock>("Date Generator");
    }
    YQ_INVOKE(reg_dategen();)
    
}

using namespace yq;

DateGen::DateGen(QWidget* parent)
    : QDialog(parent)
{
    setContentsMargins(0,0,0,0);

    QFormLayout*    lay = new QFormLayout;
    m_start     = new QDateEdit; //KDateWidget;
    m_end       = new QDateEdit; //KDateWidget;
    m_result    = new QLineEdit;
    m_result->setReadOnly(true);
    
    m_start->setDisplayFormat("yyyy-MM-dd");
    m_end->setDisplayFormat("yyyy-MM-dd");
    
    QPushButton *gen    = new QPushButton(tr("Generate"));
    connect(gen, &QPushButton::clicked, this, &DateGen::generate);
    lay->addRow(tr("Start"), m_start);
    lay->addRow(tr("End"), m_end);
    lay->addRow(gen, m_result);
    setLayout(lay);
    
    setWindowIcon(QIcon(":/icon/dice.svg"));
    setWindowTitle(tr("Date Generator"));
}

DateGen::~DateGen()
{
}

void                DateGen::generate()
{
    int s   = m_start->date().toJulianDay();
    int e   = m_end->date().toJulianDay();
    
    int     jday    = random(e,s);
    QDate   date    = QDate::fromJulianDay(jday);
    QString str     = date.toString(Qt::ISODate);
    m_result->setText(str);
}


DateGenDock::DateGenDock(QWidget*parent)
{
    setWidget(new DateGen);
}

DateGenDock::~DateGenDock()
{
}


#include "moc_DateGen.cpp"
