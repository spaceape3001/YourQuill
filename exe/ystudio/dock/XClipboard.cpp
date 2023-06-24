////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "XClipboard.hpp"
#include <basic/DelayInit.hpp>
#include <basic/TextUtils.hpp>
#include <mithrilQt/id/IdFunctor.hpp>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;

YQ_OBJECT_IMPLEMENT(XClipboard)

namespace {
    void    reg_xclipboard()
    {
        auto w = writer<XClipboard>();
        w.label("ID Clipboard");
    }
    YQ_INVOKE(reg_xclipboard();)
}

////////////////////////////////////////////////////////////////////////////////

XClipboard::XClipboard(QWidget*parent) : Dock(parent)
{
    m_model = new Model([this]()->std::vector<Id>{
        return m_data;
    });
    
    auto kCol   = column::id_key(ColOpt::Icon);
    auto nCol   = column::id_label();
    
    kCol.fnDropId   = nCol.fnDropId = [this](Id, std::span<const Id> ids) -> std::error_code {
        this -> append(ids);
        return std::error_code();
    };
    
    m_model -> addColumn(std::move(kCol));
    m_model -> addColumn(std::move(nCol));
    
    m_view  = new View(m_model);
    setWidget(m_view);
    
    setWindowTitle("Clipboard");
}

XClipboard::~XClipboard()
{
}

void        XClipboard::append(std::span<const Id> ids)
{
    std::set<Id>    lut(m_data.begin(), m_data.end());
    for(Id i : ids){
        auto [j,b]  = lut.insert(i);
        if(b)
            m_data.push_back(i);
    }
}


////////////////////////////////////////////////////////////////////////////////

XClipboard::Model::Model(IdProvider&&prov, QObject* parent) : IdModel(Type::Table, Id(), std::move(prov))
{
}

XClipboard::Model::~Model()
{
}

////////////////////////////////////////////////////////////////////////////////

XClipboard::View::View(Model*m, QWidget*parent) : IdTableView(m)
{
}

XClipboard::View::~View()
{
}

XClipboard::Model*          XClipboard::View::model()
{
    return static_cast<Model*>(m_model);
}

const XClipboard::Model*    XClipboard::View::model() const
{
    return static_cast<const Model*>(m_model);
}

////////////////////////////////////////////////////////////////////////////////

#include "moc_XClipboard.cpp"
