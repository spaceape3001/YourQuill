////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractItemModel>
#include <QTreeView>

#include <mithril/folder/Folder.hpp>
#include <mithril/preamble.hpp>

using yq::mithril::Folder;

class FileTree : public QTreeView {
    Q_OBJECT
public:
    FileTree(Folder, QWidget*parent=nullptr);
    FileTree(QWidget*parent=nullptr);
    ~FileTree();
    
    class Model;
    
    Model*          model() { return m_model; }
    const Model*    model() const { return m_model; }

private:
    Model*  m_model = nullptr;
    
    static Folder   folderOf(const QModelIndex&);
};

class FileTree::Model : public QAbstractItemModel {
    Q_OBJECT
public:
    Model(Folder , QObject*parent=nullptr);
    ~Model();

    Folder     root() const { return m_root; }
    
    QModelIndex index(int,int,const QModelIndex&) const override;
    QModelIndex parent(const QModelIndex&) const override;
    int         rowCount(const QModelIndex&) const override;
    int         columnCount(const QModelIndex&) const override;
    QVariant    data(const QModelIndex&, int) const override;
    QVariant    headerData(int, Qt::Orientation, int) const override;

private:
    Folder     m_root;
    unsigned int            m_options   = yq::mithril::cdb::HIDDEN | yq::mithril::cdb::BEST_SORT;
    
    enum Columns {
        ColIcon = 0,
        ColName,
        ColCOUNT
    };
};

