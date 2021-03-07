////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/widget/FormWidget.hpp>

class Tag;
class LineEdit;
class TagData;

class TagEdit : public FormWidget {
    Q_OBJECT
public:
    TagEdit(QWidget*parent=nullptr);
    ~TagEdit();


    void        get(TagData&) const;
    void        set(const TagData&);
    bool        readOnly() const { return m_readOnly; }
    
    
public slots:
    void        clear();
    void        setReadOnly(bool);

private:
    LineEdit   *m_name;
    LineEdit   *m_brief;
    LineEdit   *m_notes;
    LineEdit   *m_leaf; // TODO (BETTER EDIT)
    bool        m_readOnly;

};
