////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DateTimeWidget.hpp"
#include "CheckBox.hpp"
#include "DateTimeEdit.hpp"

#include <QHBoxLayout>

DateTimeWidget::DateTimeWidget(QWidget*parent) : QWidget(parent)
{
    QHBoxLayout*    lay = new QHBoxLayout;
    m_good      = new CheckBox;
    m_date      = new DateTimeEdit;
    
    m_null      = true;
    m_good->setChecked(false);
    m_date->setEnabled(false);

    setContentsMargins(0,0,0,0);
    lay->setContentsMargins(0,0,0,0);

    lay->addWidget(m_good, 0);
    lay->addWidget(m_date, 1);
    setLayout(lay);
    connect(m_good, &CheckBox::stateChanged, this, &DateTimeWidget::stateChanged);
    connect(m_date, &DateTimeEdit::editingFinished, this, &DateTimeWidget::editingFinished);

    m_date->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
}


DateTimeWidget::~DateTimeWidget()
{
}

void        DateTimeWidget::setDisplayFormat(const QString&s)
{
    m_date->setDisplayFormat(s);
}


QDateTime       DateTimeWidget::dateTime() const
{
    if(m_null)
        return QDateTime();
    return m_date->dateTime();
}

void        DateTimeWidget::stateChanged(int ch)
{
    if(ch){
        if(m_null){
            m_date->setEnabled(true);
            m_null  = false;
            emit editingFinished();
        }
    } else {
        if(!m_null){
            m_date->setEnabled(false);
            m_null  = true;
            emit    editingFinished();
        }
    }
}

void        DateTimeWidget::setDateTime(const QDateTime&dt)
{
    if(dt != dateTime()){
        if(dt.isNull()){
            m_null  = true;
            m_good  ->setChecked(false);
            m_date  ->setEnabled(false);
        } else {
            m_null  = false;
            m_good  ->setChecked(true);
            m_date  ->setEnabled(true);
            m_date  ->setDateTime(dt);
        }
    }
}

void    DateTimeWidget::setUndoCallback(UndoCallback*cb) 
{
    UndoBase::setUndoCallback(cb);
    m_good -> setUndoCallback(cb);
    m_date -> setUndoCallback(cb);
}

#include "moc_DateTimeWidget.cpp"
