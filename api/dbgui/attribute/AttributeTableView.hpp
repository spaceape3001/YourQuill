////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Attribute;
class AttributeTableModel;

class AttributeTableView : public StdTableView<Attribute> {
    Q_OBJECT
public:
    AttributeTableView(AttributeTableModel*, QWidget*parent=nullptr);
    ~AttributeTableView();
    
    AttributeTableModel*    model() const { return m_model; }
    
private:
    AttributeTableModel*    m_model;
};
