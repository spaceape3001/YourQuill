////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FileTree.hpp"
#include "IdEncoding.hpp"

#include <gluon/core/Utilities.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <QList>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;
using namespace yq::mithril::cdb;


////////////////////////////////////////////////////////////////////////////////


FileTree::Model::Model(Folder rf, QObject*parent) : QAbstractItemModel(parent), m_root(rf)
{
}

FileTree::Model::~Model()
{
}

int         FileTree::Model::columnCount(const QModelIndex& idx) const 
{
    return ColCOUNT;
}

QVariant    FileTree::Model::data(const QModelIndex& idx, int role) const 
{
    uint64_t    u   = idx.internalId();
    MType       t   = u64_type(u);
    uint64_t    id  = u64_id(u);
    
    static QIcon    folderIcon  = fetchIcon(":yq/icon/folder%1.png");
    
    switch(role){
    case Qt::DisplayRole:
        switch(idx.column()){
        case ColIcon:
            if(t == MType::Folder)
                return folderIcon;
            break;
        case ColName:
            switch(t){
            case MType::Folder:
                return QString::fromStdString(label(Folder(id)));
            case MType::Document:
                return QString::fromStdString(label(Document(id)));
            default:
                break;
            }
            break;
        }
        break;
    }
    return QVariant();
}

QVariant    FileTree::Model::headerData(int n, Qt::Orientation ori, int role) const 
{
    return QVariant();
}

QModelIndex FileTree::Model::index(int r,int c,const QModelIndex&idx) const 
{
    if((r<0) || (c<0) || (c >= ColCOUNT))
        return QModelIndex();
    Folder  fold    = u64<Folder>(idx.internalId());
    if(!fold)
        return QModelIndex();

    uint64_t    fcnt    = child_folders_count(fold, m_options);
    if((uint64_t) r >= fcnt){
        uint64_t    n   = (uint64_t) r - fcnt;
        Folder ch   = child_folder(fold, n, m_options);
        if(!ch)
            return QModelIndex();
        return createIndex(r,c,u64(ch));
    } else {
        Document ch = child_document(fold, (uint64_t) r, m_options);
        if(!ch)
            return QModelIndex();
        return createIndex(r,c,u64(ch));
    }
}

QModelIndex FileTree::Model::parent(const QModelIndex&child) const 
{
    Folder  node    = folderOf(child);
    if(!node)
        return QModelIndex();
    Folder  par     = cdb::parent(node);
    if(!par)
        return QModelIndex();
    Folder  gpp     = cdb::parent(par);
    if(!gpp)
        return QModelIndex();
    
    std::vector<Folder> all = child_folders(gpp, m_options);
    for(uint64_t i=0;i<all.size();++i)
        if(all[i] == par)
            return createIndex(i, child.column(), u64(par));
    return QModelIndex();
}

int         FileTree::Model::rowCount(const QModelIndex&idx) const 
{
    Folder  fold    = u64<Folder>(idx.internalId());
    if(!fold)
        return -1;
    
    return child_folders_count(fold, m_options) && child_documents_count(fold, m_options);
}


////////////////////////////////////////////////////////////////////////////////

Folder   FileTree::folderOf(const QModelIndex& idx)
{
    return u64<Folder>(idx.internalId());
}


FileTree::FileTree(QWidget*parent) : FileTree(top_folder(), parent)
{
}

FileTree::FileTree(Folder, QWidget*parent) : QTreeView(parent)
{
}

FileTree::~FileTree()
{
}

#include "moc_FileTree.cpp"
