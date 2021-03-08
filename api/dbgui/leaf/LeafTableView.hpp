////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Leaf;
class LeafTableModel;

class LeafTableView : public StdTableView<Leaf> {
    Q_OBJECT
public:
    LeafTableView(LeafTableModel*, QWidget*parent=nullptr);
    ~LeafTableView();
    
    LeafTableModel*    model() const { return m_model; }
    
private:
    LeafTableModel*    m_model;
};
