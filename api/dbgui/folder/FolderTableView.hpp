////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Folder;
class FolderTableModel;

class FolderTableView : public StdTableView<Folder> {
    Q_OBJECT
public:
    FolderTableView(FolderTableModel*, QWidget*parent=nullptr);
    ~FolderTableView();
    
    FolderTableModel*    model() const { return m_model; }
    
private:
    FolderTableModel*    m_model;
};
