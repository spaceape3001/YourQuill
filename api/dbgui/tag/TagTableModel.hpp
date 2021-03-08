////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/model/U64TableModel.hpp>
#include <db/Tag.hpp>
#include <dbgui/tag/TagProvider.hpp>

class TagTableModel : public U64TableModel<Tag> {
    Q_OBJECT
public:
    TagTableModel(TagProvider stp, QObject*parent=nullptr);
    ~TagTableModel();
};

