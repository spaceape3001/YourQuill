////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Property;
class PropertyTableModel;

class PropertyTableView : public StdTableView<Property> {
    Q_OBJECT
public:
    PropertyTableView(PropertyTableModel*, QWidget*parent=nullptr);
    ~PropertyTableView();
    
    PropertyTableModel*    model() const { return m_model; }
    
private:
    PropertyTableModel*    m_model;
};
