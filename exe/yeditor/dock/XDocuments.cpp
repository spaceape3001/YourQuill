////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "XDocuments.hpp"
#include <yq/core/DelayInit.hpp>
#include <yq/text/match.hpp>

#include <gluon/core/Utilities.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithrilQt/document.hpp>
#include <mithrilQt/folder.hpp>
#include <mithrilQt/id.hpp>

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
        case Folder::ID:
            return accept_folder(Folder(i.id()));
        case Document::ID:
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
    

    void    reg_xdocuments()
    {
        register_dock<XDocuments>("Documents").autoStart();
    }
    YQ_INVOKE(reg_xdocuments();)
}


////////////////////////////////////////////////////////////////////////////////

XDocuments::XDocuments(QWidget*parent) : Dock(parent)
{
    m_model = new Model();
    m_view  = new View(m_model);
    setWidget(m_view);
    
    connect(m_view, &IdTreeView::popupRequested, [this](Id i) {
        popupRequested(i);
    });
}

XDocuments::~XDocuments()
{
}


////////////////////////////////////////////////////////////////////////////////

XDocuments::Model::Model(QObject* parent) : IdModel(Type::Tree, cdb::top_folder(), files_provider())
{
    m_treeGenerator = [](Id i) -> IdProvider {
        Folder  f   = i.as<Folder>();
        if(!f)
            return {};
        return files_provider(f);
    };
    m_treeDetect    = [](Id i) -> bool {
        return i.type() == Folder::ID;
    };
    
    addFilter(accept_filter);
    
    IdColumn    col;
    col.fnDisplay     = displayFN::id_name();
    col.fnDecoration  = decorationFN::id_icon();
    addColumn(std::move(col));
    reload();
}

XDocuments::Model::~Model()
{
}

////////////////////////////////////////////////////////////////////////////////

XDocuments::View::View(Model*m, QWidget*parent) : IdTreeView(m)
{
}

XDocuments::View::~View()
{
}

XDocuments::Model*          XDocuments::View::model()
{
    return static_cast<Model*>(m_model);
}

const XDocuments::Model*    XDocuments::View::model() const
{
    return static_cast<const Model*>(m_model);
}

////////////////////////////////////////////////////////////////////////////////

#include "moc_XDocuments.cpp"
