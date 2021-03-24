////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/U64TableModel.hpp>
#include <db/Field.hpp>
#include "FieldProvider.hpp"


class FieldTableModel : public U64TableModel<Field> {
    Q_OBJECT
public:
    
    FieldTableModel(FieldProvider ctp=FieldProvider(), QObject*parent=nullptr);
    ~FieldTableModel();
};
