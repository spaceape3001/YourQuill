////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderExplorerBase.hpp"

#include <QAbstractItemModel>
#include <QIcon>
#include <QString>
#include <QTreeView>

#include <db/Folder.hpp>
#include <gui/model/StdColumn.hpp>
#include <gui/model/StdListModelImpl.hpp>
#include <gui/model/StdTableModelImpl.hpp>
#include <gui/view/StdListView.hpp>
#include <gui/view/StdTableView.hpp>
#include <util/Map.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FolderExplorerBase::Entry {
    Type            type    = Type::None;
    uint64_t        id      = 0;
    QIcon           icon;
    QString         label;
    
    Entry(){}
    Entry(Folder);
    Entry(Document);
    Entry(cdb::DocOrFold);
    
    void        set(Document);
    void        set(Folder);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FolderExplorerBase::ListModel : public StdListModel<Entry> {
    Q_OBJECT
public:
    ListModel(FolderExplorerBase* parent);
    ~ListModel();
    
public slots:
    void                refresh();

private:
    FolderExplorerBase* m_fx;
};


class FolderExplorerBase::ListView : public StdListView<Entry> {
    Q_OBJECT
public:
    ListView(ListModel*, QWidget* parent=nullptr);
    ~ListView();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FolderExplorerBase::TableModel : public StdTableModel<Entry> {
    Q_OBJECT
public:
    TableModel(FolderExplorerBase* parent);
    ~TableModel();

public slots:
    void        refresh();

private:    
    FolderExplorerBase* m_fx;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FolderExplorerBase::TableView : public StdTableView<Entry> {
    Q_OBJECT
public:
    TableView(TableModel*, QWidget*parent=nullptr);
    ~TableView();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FolderExplorerBase::TreeModel : public QAbstractItemModel {
    Q_OBJECT
public:

    TreeModel(FolderExplorerBase* parent=nullptr);
    ~TreeModel();
    
    int             columnCount(const QModelIndex&) const override;
    QVariant        data(const QModelIndex&, int) const override;
    Folder          folder(const QModelIndex&) const;
    QVariant        headerData(int,Qt::Orientation,int) const override;
    QModelIndex     index(Folder) const;
    QModelIndex     index(int row, int column, const QModelIndex&) const override;
    QModelIndex     parent(const QModelIndex&) const;
    int             rowCount(const QModelIndex&) const override;
    
    //  TODO ... refresh comes later
public slots:
    void        refresh();
    
private:
    struct Node;

    Map<Folder,Node*>           m_nodes;
    FolderExplorerBase*         m_fx;
    
    Node*                       node(Folder);
    const Node*                 node(Folder) const;
    Node*                       node(const QModelIndex&);
    const Node*                 node(const QModelIndex&) const;
    bool        m_showHidden    = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class FolderExplorerBase::TreeView : public QTreeView {
    Q_OBJECT
public:
    TreeView(TreeModel*, QWidget*parent=nullptr);
    ~TreeView();
    
    TreeModel*          model() { return m_model; }
    const TreeModel*    model() const { return m_model; }

    Folder              currentFolder() const;
    void                setCurrentFolder(Folder);
    
private:
    TreeModel*      m_model;
};
