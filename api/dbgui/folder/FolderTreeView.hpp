////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QTreeView>

class FolderTreeModel;

class FolderTreeView : public QTreeView {
    Q_OBJECT
public:
    FolderTreeView(FolderTreeModel*, QWidget*parent=nullptr);
    ~FolderTreeView();
    
    FolderTreeModel*        model() { return m_model; }
    const FolderTreeModel*  model() const { return m_model; }

private:
    FolderTreeModel*        m_model;
};
