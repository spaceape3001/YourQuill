////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TagTableModel.hpp"
#include "TagTableView.hpp"
#include <gui/model/StdTableModelImpl.hpp>
#include <QHeaderView>

template class StdTableModel<Tag>;  // explicitly instantiate the template here

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

TagTableModel::TagTableModel(TagProvider stp, QObject* parent) : StdTableModel<Tag>(parent), m_source(stp)
{
    customRO("ID",  [](Tag t) -> auto {
        return (quint64) t.id;
    });
    
    customRO("Key", [](Tag t) -> auto {
        return cdb::key(t);
    });
    
    customRW("Name", [](Tag t) -> QString {
        return cdb::name(t);
    }, [](Tag& t, const QString& k) -> bool {
        return cdb::set_name(t, k.simplified());
    });
    
    customRW("Brief", [](Tag t) -> QString {
        return cdb::brief(t);
    }, [](Tag& t, const QString& k) -> bool {
        return cdb::set_brief(t, k.simplified());
    });

    _refresh();
}

TagTableModel::~TagTableModel()
{
}

void        TagTableModel::_refresh()
{
    m_rows      = m_source -> all();
    m_tags      = makeSet(m_rows);
}

void        TagTableModel::append(const Tag& t)
{
    if(t == Tag())
        return ;
    if(m_tags.has(t))
        return ;
    
    m_tags << t;
    Base::append(t);
}

void        TagTableModel::check()
{
    Vector<Tag> tags    = m_source -> all();
    if(tags.empty()){       // nothing from nothing....
        if(m_rows.empty())
            return ;
        clear();
        return ;
    }
    
    Set<Tag>    ctags   = makeSet(tags);
    Set<Tag>    ntags   = ctags - m_tags;
    Set<Tag>    rtags   = m_tags - ctags;
    Set<Tag>    utags   = ctags & m_tags;
    
        // alright, setup the vector for *NEW* things
    tags.erase_if([&](Tag t) -> bool { 
        return m_tags.has(t);
    });
    
    removeRowIf([&](Tag t) -> bool {
        return rtags.has(t);
    });
    
    allChanged();
    
    m_tags  = ctags;
    Base::append(tags);
}


void        TagTableModel::refresh()
{
    beginResetModel();
    _refresh();
    endResetModel();
}


Tag         TagTableModel::tag(int r) const
{
    if((r < 0) || (r >= (int) m_rows.size()))
        return Tag{};
    return m_rows[r];
}

Tag         TagTableModel::tag(const QModelIndex&idx) const
{
    return tag(idx.row());
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


TagTableView::TagTableView(TagTableModel* m, QWidget*parent) : StdTableView<Tag>(m, parent), m_model(m)
{
    setModel(m);
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
}

TagTableView::~TagTableView()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_TagTableModel.cpp"
#include "moc_TagTableView.cpp"
