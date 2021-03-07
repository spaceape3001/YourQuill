////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TagEdit.hpp"

#include <db/TagData.hpp>
#include <gui/edit/LineEdit.hpp>

TagEdit::TagEdit(QWidget*parent) : FormWidget(parent), m_readOnly(false)
{
    m_name  = new LineEdit;
    m_leaf  = new LineEdit;
    m_brief = new LineEdit;
    m_notes = new LineEdit;
    
    addRow(tr("Name"), m_name);
    addSeparator();
    addRow(tr("Brief"), m_brief);
    addRow(tr("Leaf"), m_leaf);
    addRow(tr("Notes"), m_notes);
}

TagEdit::~TagEdit()
{
}

void        TagEdit::clear()
{
    m_name -> clear();
    m_leaf -> clear();
    m_brief -> clear();
    m_notes -> clear();
}

void        TagEdit::get(TagData&v) const
{
    v.name  = m_name -> text();
    v.leaf  = m_leaf -> text();
    v.brief = m_brief -> text();
    v.notes = m_notes -> text();
}

void        TagEdit::set(const TagData&v)
{
    m_name -> setText(v.name.qString());
    m_leaf -> setText(v.leaf.qString());
    m_brief -> setText(v.brief.qString());
    m_notes -> setText(v.notes.qString());
}

void        TagEdit::setReadOnly(bool f)
{
    m_name -> setReadOnly(f);
    m_leaf -> setReadOnly(f);
    m_brief -> setReadOnly(f);
    m_notes -> setReadOnly(f);
    m_readOnly = f;
}


#include "moc_TagEdit.cpp"
