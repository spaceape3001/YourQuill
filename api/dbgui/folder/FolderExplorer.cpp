////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FolderExplorerBase.hpp"
#include "FolderExplorerImpl.hpp"

#include <gui/StdIcons.hpp>
#include <util/DiffEngine.hpp>
#include <util/Reverse.hpp>
#include <util/Vector.hpp>

#include <QComboBox>
#include <QHBoxLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QToolBar>
#include <QToolButton>

static_assert(sizeof(void*) >= sizeof(uint64_t), "MUST BE at laest 64-bit code!");

template class StdListModel<FolderExplorerBase::Entry>;
template class StdListView<FolderExplorerBase::Entry>;
template class StdTableModel<FolderExplorerBase::Entry>;
template class StdTableView<FolderExplorerBase::Entry>;


FolderExplorerBase::FolderExplorerBase(QWidget*parent) : SubWin(parent)
{
    m_url       = new QComboBox;
    m_url -> setEditable(true);
    
    m_listModel     = new ListModel(this);
    m_listView      = new ListView(m_listModel);
    m_tableModel    = new TableModel(this);
    m_tableView     = new TableView(m_tableModel);
    m_treeModel     = new TreeModel(this);
    m_treeView      = new TreeView(m_treeModel);
    
    m_stacked       = new QStackedWidget;
    m_listPos       = m_stacked -> addWidget(m_listView);
    m_tablePos      = m_stacked -> addWidget(m_tableView);

    QToolBar*       bar = new QToolBar;
    bar -> addAction(si_go_back(), tr("Back"), this, &FolderExplorerBase::cmdGoBack);
    bar -> addAction(si_go_forward(), tr("Fwd"), this, &FolderExplorerBase::cmdGoForward);
    //bar -> addAction(si_view_refresh(), tr("Reload"), m_view, &WebBase::View::reload);
    //bar -> addAction(si_go_home(), tr("Home"), this, &WebBase::goHome);
    bar -> addWidget(m_url);
    
    QSplitter   *split  = new QSplitter(Qt::Horizontal);
    split -> addWidget(m_treeView);
    split -> addWidget(m_stacked);
    split -> setStretchFactor(0, 1);
    split -> setStretchFactor(1, 4);
    
    QVBoxLayout*    vlay    = new QVBoxLayout(this);
    vlay -> addWidget(bar, 0);
    vlay -> addWidget(split, 1);
    
    changeTo(cdb::top_folder());
}

FolderExplorerBase::~FolderExplorerBase()
{
}

void    FolderExplorerBase::changeTo(Folder)
{
}

void    FolderExplorerBase::cmdGoBack()
{
}

void    FolderExplorerBase::cmdGoForward()
{
}

void    FolderExplorerBase::cmdGoUp()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct FolderExplorerBase::NameCol : public StdColumn<Entry> {
    
    QVariant display(const Entry& e) const override
    {
        return e.label;
    }
    
    QVariant decoration(const Entry& e) const override
    {
        return e.icon;
    } 
    
    NameCol() : StdColumn<Entry>("Name") {}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FolderExplorerBase::ListModel::ListModel(QObject*parent) : StdListModel<Entry>(parent)
{
    col(new NameCol);
}

FolderExplorerBase::ListModel::~ListModel()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FolderExplorerBase::ListView::ListView(ListModel* lm, QWidget* parent) : StdListView<Entry>(lm, parent)
{
}

FolderExplorerBase::ListView::~ListView()
{
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FolderExplorerBase::TableModel::TableModel(QObject*parent) : StdTableModel<Entry>(parent)
{
    col(new NameCol);
}

FolderExplorerBase::TableModel::~TableModel()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FolderExplorerBase::TableView::TableView(TableModel*tm, QWidget*parent) : StdTableView<Entry>(tm, parent)
{
}

FolderExplorerBase::TableView::~TableView()
{
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FolderExplorerBase::TreeModel::Node {
    TreeModel*              const  ftm;
    const Folder            folder;
    const Folder            parent;
    Image                   icoImg;
    QIcon                   icon;
    QString                 label;
    Vector<Folder>          children;
    
    enum {
        CHG_DATA            = 1,
        CHG_CHILDREN        = 2
    };
    
    
    Node(TreeModel* m, Folder f) : ftm(m), folder(f), parent(cdb::parent(f))
    {
    }

    void    refresh(unsigned chgSignal = 3)
    {
        Vector<Folder>  nc  = cdb::folders(folder, Sorted::YES);
        if(chgSignal&CHG_CHILDREN){
            auto chg    = diff_it(children, nc);
            if(any_changes(chg)){
                QModelIndex par = ftm->index(parent);
            
                for(auto& de : reverse(chg)){
                    if(de.mode == DEResult::Remove){
                        ftm->beginRemoveRows(par, de.left.low, de.left.high-1);
                        children.erase_range(de.left.low, de.left.high);
                        ftm->endRemoveRows();
                    }
                }
                
                for(auto& de : chg){
                    if(de.mode == DEResult::Add){
                        ftm->beginInsertRows(par, de.right.low, de.right.high-1);
                        children.insert(children.begin() + de.right.low, nc.begin()+de.right.low, nc.begin()+de.right.high);
                        ftm->endInsertRows();
                    }
                }
            }
        } else {
            children    = nc;
        }
        
        Image ni        = cdb::icon(folder);
        QString nl      = cdb::label(folder);
        bool        chg = (ni != icoImg) || (nl != label);
        
        if(ni!=icoImg){
            icoImg      = ni;
            icon        = cdb::qIcon(ni);
            if(icon.isNull())
                icon    = si_folder();
        }
        if(nl != label)
            label       = nl;
        if(chg){
            QModelIndex i   = ftm -> index(folder);
            ftm -> dataChanged(i,i);
        }
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

Folder   FolderExplorerBase::TreeModel::folder(const QModelIndex&idx) const
{
    return Folder{reinterpret_cast<uint64_t>(idx.internalPointer())};
}

FolderExplorerBase::TreeModel::TreeModel(QObject*parent) : QAbstractItemModel(parent)
{
}

FolderExplorerBase::TreeModel::~TreeModel()
{
    for(auto& i : m_nodes)
        delete i.second;
    m_nodes.clear();
}

int         FolderExplorerBase::TreeModel::columnCount(const QModelIndex&) const 
{
    return 1;
}

QVariant    FolderExplorerBase::TreeModel::data(const QModelIndex& idx, int role) const 
{
    const Node* n   = node(idx);
    
    switch(role){
    case Qt::DisplayRole:
        if(n && n->folder)
            return cdb::label(n->folder);
        break;
    case Qt::DecorationRole:
        if(n)
            return QVariant::fromValue(n->icon);
        break;
    }
    return QVariant();
}

QModelIndex FolderExplorerBase::TreeModel::index(Folder f) const
{
    const Node*n    = node(f);
    if(!n)
        return QModelIndex();
    const Node*p    = node(n->parent);
    if(!p)
        return QModelIndex();
    return createIndex((int) p->children.first_index_of(f), 0, (void*) f.id);
    
}

QModelIndex FolderExplorerBase::TreeModel::index(int row, int column, const QModelIndex&par) const 
{
    Folder      f   = par.isValid() ? folder(par) : cdb::top_folder();
    const Node* n   = node(f);
    if(!n)
        return QModelIndex();
    if((row < 0) || (row>= (int) n->children.size()))
        return QModelIndex();
    return createIndex(row, column, (void*) n->children[row].id);
    
}

FolderExplorerBase::TreeModel::Node*          FolderExplorerBase::TreeModel::node(Folder f)
{
    Node* n         = m_nodes.get(f,nullptr);
    if(f && !n){
        n   = new Node(this, f);
        n -> refresh(0);
        m_nodes[f]  = n;
    }
    return n;
}

const FolderExplorerBase::TreeModel::Node*    FolderExplorerBase::TreeModel::node(Folder f) const
{
    return const_cast<TreeModel*>(this)->node(f);
}

FolderExplorerBase::TreeModel::Node*          FolderExplorerBase::TreeModel::node(const QModelIndex& idx)
{
    return node(folder(idx));
}

const FolderExplorerBase::TreeModel::Node*    FolderExplorerBase::TreeModel::node(const QModelIndex& idx) const
{
    return node(folder(idx));
}


QModelIndex FolderExplorerBase::TreeModel::parent(const QModelIndex& idx) const
{
    Folder  f   = folder(idx);
    if(!f)
        return QModelIndex();
    const Node* n     = node(f);
    if(!n || !n->parent)
        return QModelIndex();
    const Node* p    = node(n->parent);
    if(!p || !p->parent)
        return QModelIndex();
    const Node* gp    = node(p->parent);
    if(!gp)
        return QModelIndex();
    int row = gp->row(n->parent);
    return createIndex(row, idx.column(), (void*) n->parent.id);
}

int         FolderExplorerBase::TreeModel::rowCount(const QModelIndex&idx) const 
{
    const Node* n   = node(idx);
    if(!n)
        return 0;
    
    return (int) n -> children.size();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FolderExplorerBase::TreeView::TreeView(TreeModel*ftm, QWidget*parent) : QTreeView(parent), m_model(ftm)
{
    setModel(ftm);
}

FolderExplorerBase::TreeView::~TreeView()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "moc_FolderExplorerBase.cpp"
#include "moc_FolderExplorerImpl.cpp"
