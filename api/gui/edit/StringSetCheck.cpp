////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "StringSetCheck.hpp"
#include "CheckBox.hpp"
#include <gui/layout/FlowLayout.hpp>
#include <util/Set.hpp>
#include <util/Utilities.hpp>
#include <util/Vector.hpp>

StringSetCheck::StringSetCheck(QWidget* parent) : QWidget(parent)
{
    m_layout        = new FlowLayout;
    setLayout(m_layout);
}

StringSetCheck::~StringSetCheck()
{
}
    
void                StringSetCheck::setItems(const StringSet& items)
{
    setItems(qt(items));
}

void                StringSetCheck::setItems(const QStringSet& items)
{
    if(makeVector(items) != m_checks.keys()){
        m_layout -> clear();
        for(QString i : items){
            CheckBox*   cb  = new CheckBox(i);
            m_checks[i] = cb;
            m_layout -> addWidget(cb);
            connect(cb, &QCheckBox::stateChanged, this, &StringSetCheck::changed);
        }
    } else {
        for(auto& i : m_checks)
            i.second -> setChecked(false);
    }
}

void                StringSetCheck::setChecked(const StringSet& items)
{
    setChecked(qt(items));
}

void                StringSetCheck::setChecked(const QStringSet& items)
{
    for(auto& i : m_checks){
        if(!i.second -> isTristate())
            i.second -> setChecked(items.has(i.first));
    }
}

void                StringSetCheck::setFixed(const StringSet& items)
{
    setFixed(qt(items));
}

void                StringSetCheck::setFixed(const QStringSet& items)
{
    for(auto& i : m_checks){
        if(items.has(i.first)){
            i.second -> setTristate(true);
            i.second -> setCheckState(Qt::PartiallyChecked);
        } else {
            if(i.second -> checkState() == Qt::PartiallyChecked)
                i.second -> setCheckState(Qt::Unchecked);
            i.second -> setTristate(false);
        }
    }
}


QStringSet StringSetCheck::checked(bool fIncFixed) const
{
    QStringSet ret;
    if(fIncFixed){
        for(auto& i : m_checks)
            if(i.second -> checkState() != Qt::Unchecked)
                ret << i.first;
    } else {
        for(auto& i : m_checks)
            if(i.second -> checkState() == Qt::Checked)
                ret << i.first;
    }
    return ret;
}

void       StringSetCheck::clear()
{
    for(auto & i : m_checks)
        i.second -> setChecked(false);
}

void        StringSetCheck::setReadOnly(bool f)
{
    setEnabled(!f);
    for(auto& i : m_checks)
        i.second -> setEnabled(!f);
}

#include "moc_StringSetCheck.cpp"

