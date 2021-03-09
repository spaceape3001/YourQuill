////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Directory;
class DirectoryTableModel;

class DirectoryTableView : public StdTableView<Directory> {
    Q_OBJECT
public:
    DirectoryTableView(DirectoryTableModel*, QWidget*parent=nullptr);
    ~DirectoryTableView();
    
    DirectoryTableModel*    model() const { return m_model; }
    
private:
    DirectoryTableModel*    m_model;
};
