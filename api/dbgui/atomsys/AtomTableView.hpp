////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Atom;
class AtomTableModel;

class AtomTableView : public StdTableView<Atom> {
    Q_OBJECT
public:
    AtomTableView(AtomTableModel*, QWidget*parent=nullptr);
    ~AtomTableView();
    
    AtomTableModel*    model() const { return m_model; }
    
private:
    AtomTableModel*    m_model;
};
