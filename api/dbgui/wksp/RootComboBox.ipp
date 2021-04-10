////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "RootComboBox.hpp"
#include <db/FileSys.hpp>
#include <db/Root.hpp>
#include <db/Workspace.hpp>

RootComboBox::RootComboBox(QWidget*parent) : ComboBox(parent), m_writable(false)
{
    _update();
}

RootComboBox::~RootComboBox()
{
}

void        RootComboBox::setDataRole(DataRole dr)
{
    m_role  = dr;
    _update();
}

void        RootComboBox::setWritableRoots(bool f)
{
    m_writable = f;
    _update();
}

const Root* RootComboBox::currentRoot() const
{
    QVariant    v   = currentData();
    if(!v.isValid())
        return nullptr;
    return wksp::root((uint64_t) v.toULongLong());
}

void        RootComboBox::setCurrentRoot(const Root*rt)
{
    if(!rt)
        return ;
    int ic   = count();
    for(int i=0;i<ic;++i){
        if((uint64_t) itemData(i).toULongLong() == rt->id()){
            setCurrentIndex(i);
            return ;
        }
    }
}


void        RootComboBox::_update()
{
    const Root*old    = currentRoot();
    clear();
    const auto& m   = m_writable ? wksp::root_writes(m_role) : wksp::root_reads(m_role);
    for(const Root*r : m)
        addItem(tr("(%1) %2").arg(r->key()).arg(r->name()), (quint64) r->id());
    setCurrentRoot(old);
    if(!currentRoot() && m_writable)
        setCurrentRoot(wksp::root_first(m_role));
}

