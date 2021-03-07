////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Tag;

class TagTableModel;

class TagTableView : public StdTableView<Tag> {
    Q_OBJECT
public:
    TagTableView(TagTableModel*, QWidget*parent=nullptr);
    ~TagTableView();
    
    TagTableModel*  model() const { return m_model; }
    
private:
    TagTableModel*      m_model;
};

