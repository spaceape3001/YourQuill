////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractItemModel>
#include <db/Folder.hpp>
#include <util/Map.hpp>

class FolderTreeView;

class FolderTreeModel : public QAbstractItemModel {
    Q_OBJECT
public:

    FolderTreeModel(QObject* parent=nullptr);
    FolderTreeModel(Folder, QObject*parent=nullptr);
    ~FolderTreeModel();
    
    int             columnCount(const QModelIndex&) const override;
    QVariant        data(const QModelIndex&, int) const override;
    Folder          folder(const QModelIndex&) const;
    QModelIndex     index(Folder);
    QModelIndex     index(int row, int column, const QModelIndex&) const override;
    QModelIndex     parent(const QModelIndex&) const;
    int             rowCount(const QModelIndex&) const override;
    
    //  TODO ... refresh comes later
    
private:
    struct Node;
    Folder                      m_root;
    mutable Map<Folder,Node>    m_nodes;
    
    const Node&                 node(Folder) const;
    Node&                       node(Folder);
    Node*                       node_ptr(const QModelIndex&);
    const Node*                 node_ptr(const QModelIndex&) const;
};
