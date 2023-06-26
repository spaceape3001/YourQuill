////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "XFragments.hpp"
#include <basic/DelayInit.hpp>
#include <basic/TextUtils.hpp>
#include <gluon/core/Utilities.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/root/Root.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithrilQt/fragment.hpp>
#include <mithrilQt/directory.hpp>
#include <mithrilQt/id.hpp>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;

namespace {

    bool        accept_fragment(Fragment frag)
    {
        auto ext = cdb::suffix(frag);
        if(is_similar(ext.ext, "tmp"))
            return false;
    
        return true;
    }
    
    bool        accept_directory(Directory f)
    {
        return true;
    }

    bool        accept_filter(Id i)
    {
        switch(i.type()){
        case Directory::ID:
            return accept_directory(Directory(i.id()));
        case Fragment::ID:
            return accept_fragment(Fragment(i.id()));
        default:
            return false;
        }
    }

    IdProvider   files_provider(Directory dir)
    {
        return [dir]() -> std::vector<Id> {
            std::vector<Id> ret;
            std::vector<Directory> a    = cdb::child_directories(dir, Sorted::YES);
            std::vector<Fragment> b     = cdb::child_fragments(dir, Sorted::YES);
            ret.reserve(a.size()+b.size());
            ret.insert(ret.end(), a.begin(), a.end());
            ret.insert(ret.end(), b.begin(), b.end());
            return ret;
        };
    }
    
    IdProvider   files_provider()
    {
        return []() -> std::vector<Id> {
            std::vector<Id> ret;
            for(Root r : cdb::all_roots()){
                Directory d = cdb::directory(r);
                if(d)
                    ret.push_back(d);
            }
            return ret;
        };
    }
    
    QVariant    col_path(Id i)
    {
        switch(i.type()){
        case Directory::ID: 
            {
                Directory   d(i.id());
                if(cdb::folder(d) == cdb::top_folder())
                    return qString(cdb::path(d).string());
                else
                    return qString(cdb::name(d));
            }
            break;
        case Fragment::ID:
            return qString(cdb::name(Fragment(i.id())));
        default:
            return QVariant();
        }
    }

    void    reg_xfiles()
    {
        register_dock<XFragments>("Fragments").autoStart();
    }
    YQ_INVOKE(reg_xfiles();)
}


////////////////////////////////////////////////////////////////////////////////

XFragments::XFragments(QWidget*parent) : Dock(parent)
{
    m_model = new Model();
    m_view  = new View(m_model);
    setWidget(m_view);
    
    connect(m_view, &IdTreeView::popupRequested, [this](Id i) {
        popupRequested(i);
    });
}

XFragments::~XFragments()
{
}


////////////////////////////////////////////////////////////////////////////////

XFragments::Model::Model(QObject* parent) : IdModel(Type::Tree, Id(), files_provider())
{
    m_treeGenerator = [](Id i) -> IdProvider {
        Directory  f   = i.as<Directory>();
        if(!f)
            return {};
        return files_provider(f);
    };
    m_treeDetect    = [](Id i) -> bool {
        return i.type() == Directory::ID;
    };
    
    addFilter(accept_filter);
    
    IdColumn    col;
    col.fnDisplay     = col_path;
    col.fnDecoration  = decorationFN::id_icon();
    addColumn(std::move(col));
    reload();
}

XFragments::Model::~Model()
{
}

////////////////////////////////////////////////////////////////////////////////

XFragments::View::View(Model*m, QWidget*parent) : IdTreeView(m)
{
}

XFragments::View::~View()
{
}

XFragments::Model*          XFragments::View::model()
{
    return static_cast<Model*>(m_model);
}

const XFragments::Model*    XFragments::View::model() const
{
    return static_cast<const Model*>(m_model);
}

////////////////////////////////////////////////////////////////////////////////

#include "moc_XFragments.cpp"
