////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/U64TableModel.hpp>
#include <db/Attribute.hpp>
#include "AttributeProvider.hpp"


class AttributeTableModel : public U64TableModel<Attribute> {
    Q_OBJECT
public:
    
    AttributeTableModel(AttributeProvider atp=AttributeProvider(), QObject*parent=nullptr);
    ~AttributeTableModel();
};
