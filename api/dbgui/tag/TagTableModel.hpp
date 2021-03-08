////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/Tag.hpp>
#include <dbgui/tag/TagProvider.hpp>
#include <gui/model/StdTableModel.hpp>
#include <util/Set.hpp>

class TagTableModel : public StdTableModel<Tag> {
    Q_OBJECT
public:

    TagTableModel(TagProvider stp, QObject*parent=nullptr);
    ~TagTableModel();
    
    void            append(const Tag&) override;
    Tag             tag(int) const;
    Tag             tag(const QModelIndex&) const;
    
public slots:
    void            refresh();
    void            check();

private:    
    void            _refresh();
    
    TagProvider     m_source;
    Set<Tag>        m_tags;     // from the last pull
};

