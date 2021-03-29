////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/U64TableModel.hpp>
#include <db/Property.hpp>
#include "PropertyProvider.hpp"


class PropertyTableModel : public U64TableModel<Property> {
    Q_OBJECT
public:
    
    PropertyTableModel(PropertyProvider ctp=PropertyProvider(), QObject*parent=nullptr);
    ~PropertyTableModel();
};
