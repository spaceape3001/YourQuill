////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FolderTreeModel.hpp"
#include "FolderTreeView.hpp"

#include <gui/StdIcons.hpp>
#include <util/Vector.hpp>

static_assert(sizeof(void*) >= sizeof(uint64_t), "MUST BE at laest 64-bit code!");


struct FolderTreeModel::Node {
    Folder          folder;
    Folder          parent;
    QIcon           icon;
    QString         label;
    Vector<Folder>  children;
    
    Node()
    {
    }
    
    Node&   operator=(Folder f)
    {
        folder      = f;
        parent      = cdb::parent(f);
        refresh();
        return *this;
    }

    void            refresh()
    {
        children    = cdb::folders(folder, Sorted::YES);
        icon        = cdb::qIcon(cdb::icon(folder));
        if(icon.isNull())
            icon    = si_folder();
        label       = cdb::label(folder);
    }
    
    int             row(Folder f) const
    {
        auto i = children.find(f);
        if(i != children.end())
            return (int)(i-children.begin());
        return -1;
    }
    
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Folder   FolderTreeModel::folder(const QModelIndex&idx) const
{
    return Folder{reinterpret_cast<uint64_t>(idx.internalPointer())};
}

FolderTreeModel::FolderTreeModel(QObject* parent) : FolderTreeModel(cdb::top_folder(), parent)
{
}

FolderTreeModel::FolderTreeModel(Folder f, QObject*parent) : QAbstractItemModel(parent), m_root(f)
{
    if(!m_root)
        m_root  = cdb::top_folder();
}

FolderTreeModel::~FolderTreeModel()
{
}

int         FolderTreeModel::columnCount(const QModelIndex&) const 
{
    return 1;
}

QVariant    FolderTreeModel::data(const QModelIndex& idx, int role) const 
{
    const Node* n   = node_ptr(idx);
    
    switch(role){
    case Qt::DisplayRole:
        if(n && n->folder)
            return cdb::label(n->folder);
        break;
    case Qt::DecorationRole:
        break;
    }
    return QVariant();
}

QModelIndex FolderTreeModel::index(int row, int column, const QModelIndex&par) const 
{
    if(!m_root)
        return QModelIndex();
    Folder      f   = par.isValid() ? folder(par) : m_root;
    const Node& n   = node(f);
    if((row < 0) || (row>= (int) n.children.size()))
        return QModelIndex();
    return createIndex(row, column, (void*) n.children[row].id);
    
}

const FolderTreeModel::Node&    FolderTreeModel::node(Folder f) const
{
    auto i = m_nodes.find(f);
    if(i != m_nodes.end())
        return i->second;
    return m_nodes[f] = f;
}

FolderTreeModel::Node&          FolderTreeModel::node(Folder f)
{
    auto i = m_nodes.find(f);
    if(i != m_nodes.end())
        return i->second;
    return m_nodes[f] = f;
}

FolderTreeModel::Node*          FolderTreeModel::node_ptr(const QModelIndex& idx)
{
    Folder  f   = folder(idx);
    if(f)
        return &node(f);
    return nullptr;
}

const FolderTreeModel::Node*    FolderTreeModel::node_ptr(const QModelIndex& idx) const
{
    Folder  f   = folder(idx);
    if(f)
        return &node(f);
    return nullptr;
}


QModelIndex FolderTreeModel::parent(const QModelIndex& idx) const
{
    Folder  f   = folder(idx);
    if(!f)
        return QModelIndex();
    const Node& n     = node(f);
    if(!n.parent)
        return QModelIndex();
    const Node& p    = node(n.parent);
    if(!p.parent)
        return QModelIndex();
    const Node& gp    = node(p.parent);
    int row = gp.row(n.parent);
    return createIndex(row, idx.column(), (void*) n.parent.id);
}

int         FolderTreeModel::rowCount(const QModelIndex&idx) const 
{
    const Node* n   = node_ptr(idx);
    if(!n)
        return 0;
    return (int) n -> children.size();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FolderTreeView::FolderTreeView(FolderTreeModel*ftm, QWidget*parent) : QTreeView(parent), m_model(ftm)
{
    setModel(ftm);
}

FolderTreeView::~FolderTreeView()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "moc_FolderTreeModel.cpp"
#include "moc_FolderTreeView.cpp"
