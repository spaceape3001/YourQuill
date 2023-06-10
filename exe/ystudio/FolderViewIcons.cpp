////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FolderViewIcons.hpp"
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>

using namespace yq;
//using namespace yq::gluon;
using namespace yq::mithril;
using namespace yq::mithril::cdb;

FolderViewIcons::Model::Model(Folder fold, QObject*parent) : QAbstractListModel(parent), m_folder(fold)
{
}

FolderViewIcons::Model::~Model()
{
}


QVariant    FolderViewIcons::Model::data(const QModelIndex&idx, int r) const
{
    if(r == Qt::DisplayRole)
        return idx.row();
    return QVariant();
}

int         FolderViewIcons::Model::rowCount(const QModelIndex&) const
{
    return child_folders_count(m_folder) + child_documents_count(m_folder);
}

////////////////////////////////////////////////////////////////////////////////

FolderViewIcons::FolderViewIcons(Folder f, QWidget*parent) : QListView(parent)
{
    m_model = new Model(f, this);
    setModel(m_model);
}

FolderViewIcons::FolderViewIcons(QWidget*parent) : FolderViewIcons(top_folder(), parent)
{
}

FolderViewIcons::~FolderViewIcons()
{
}

Folder          FolderViewIcons::folder() const
{
    return m_model -> folder();
}
