////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Class;
class ClassTableModel;

class ClassTableView : public StdTableView<Class> {
    Q_OBJECT
public:
    ClassTableView(ClassTableModel*, QWidget*parent=nullptr);
    ~ClassTableView();
    
    ClassTableModel*    model() const { return m_model; }
    
private:
    ClassTableModel*    m_model;
};
