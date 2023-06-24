////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "XFiles.hpp"
#include <basic/DelayInit.hpp>
#include <basic/TextUtils.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithrilQt/document/DocumentColumn.hpp>
#include <mithrilQt/folder/FolderColumn.hpp>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;

namespace {

    bool        accept_document(Document d)
    {
        auto ext = cdb::suffix(d);
        if(is_similar(ext.ext, "tmp"))
            return false;
    
        return true;
    }
    
    bool        accept_folder(Folder f)
    {
        return true;
    }

    bool        accept_filter(Id i)
    {
        switch(i.type()){
        case IdType::Folder:
            return accept_folder(Folder(i.id()));
        case IdType::Document:
            return accept_document(Document(i.id()));
        default:
            return false;
        }
    }

    IdProvider   files_provider(Folder f)
    {
        return [f]() -> std::vector<Id> {
            std::vector<Id> ret;
            std::vector<Folder> a       = cdb::child_folders(f, Sorted::YES);
            std::vector<Document> b     = cdb::child_documents(f, Sorted::YES);
            ret.reserve(a.size()+b.size());
            ret.insert(ret.end(), a.begin(), a.end());
            ret.insert(ret.end(), b.begin(), b.end());
            return ret;
        };
    }
    
    IdProvider   files_provider()
    {
        return files_provider(cdb::top_folder());
    }
    
    QVariant    fd_label(Id i)
    {
        switch(i.type()){
        case IdType::Folder:
            return qString(cdb::name(Folder(i.id())));
        case IdType::Document:
            return qString(cdb::name(Document(i.id())));
        default:
            return QVariant();
        }
    }
    
    QVariant    fd_icon(Id i)
    {
        switch(i.type()){
        case IdType::Folder:
            return qIcon(Folder(i.id()));
        case IdType::Document:
            return qIcon(Document(i.id()));
        default:
            return QVariant();
        }
    }
    
    void    reg_xfiles()
    {
        register_dock<XFiles>("File Explorer").autoStart();
    }
    YQ_INVOKE(reg_xfiles();)
}


////////////////////////////////////////////////////////////////////////////////

XFiles::XFiles(QWidget*parent) : Dock(parent)
{
    m_model = new Model();
    m_view  = new View(m_model);
    setWidget(m_view);
}

XFiles::~XFiles()
{
}


////////////////////////////////////////////////////////////////////////////////

XFiles::Model::Model(QObject* parent) : IdModel(Type::Tree, cdb::top_folder(), files_provider())
{
    m_treeGenerator = [](Id i) -> IdProvider {
        Folder  f   = i.as<Folder>();
        if(!f)
            return {};
        return files_provider(f);
    };
    m_treeDetect    = [](Id i) -> bool {
        return i.type() == IdType::Folder;
    };
    
    addFilter(accept_filter);
    
    IdColumn    col;
    col.fnDisplay     = fd_label;
    col.fnDecoration  = fd_icon;
    addColumn(std::move(col));
    reload();
}

XFiles::Model::~Model()
{
}

////////////////////////////////////////////////////////////////////////////////

XFiles::View::View(Model*m, QWidget*parent) : IdTreeView(m)
{
}

XFiles::View::~View()
{
}

XFiles::Model*          XFiles::View::model()
{
    return static_cast<Model*>(m_model);
}

const XFiles::Model*    XFiles::View::model() const
{
    return static_cast<const Model*>(m_model);
}

////////////////////////////////////////////////////////////////////////////////

#include "moc_XFiles.cpp"
