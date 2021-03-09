////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Field;
class FieldTableModel;

class FieldTableView : public StdTableView<Field> {
    Q_OBJECT
public:
    FieldTableView(FieldTableModel*, QWidget*parent=nullptr);
    ~FieldTableView();
    
    FieldTableModel*    model() const { return m_model; }
    
private:
    FieldTableModel*    m_model;
};
