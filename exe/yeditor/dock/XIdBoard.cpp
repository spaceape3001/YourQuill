////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "XIdBoard.hpp"
#include <yq/basic/DelayInit.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithrilQt/id.hpp>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;

namespace {
    void    reg_xclipboard()
    {
        register_dock<XIdBoard>("ID Board").area(Qt::RightDockWidgetArea);
    }
    YQ_INVOKE(reg_xclipboard();)
}

////////////////////////////////////////////////////////////////////////////////

XIdBoard::XIdBoard(QWidget*parent) : Dock(parent)
{
    m_model = new Model([this]()->std::vector<Id>{
        return m_data;
    });
    
    auto kCol   = column::id_key(ColOpt::Icon);
    auto nCol   = column::id_name();
    
    kCol.fnDropId   = nCol.fnDropId = [this](Id, std::span<const Id> ids) -> std::error_code {
        this -> append(ids);
        return std::error_code();
    };
    
    m_model -> addColumn(std::move(kCol));
    m_model -> addColumn(std::move(nCol));
    
    m_view  = new View(m_model);
    setWidget(m_view);
}

XIdBoard::~XIdBoard()
{
}

void        XIdBoard::append(std::span<const Id> ids)
{
    std::set<Id>    lut(m_data.begin(), m_data.end());
    for(Id i : ids){
        auto [j,b]  = lut.insert(i);
        if(b)
            m_data.push_back(i);
    }
}


////////////////////////////////////////////////////////////////////////////////

XIdBoard::Model::Model(IdProvider&&prov, QObject* parent) : IdModel(Type::Table, Id(), std::move(prov))
{
}

XIdBoard::Model::~Model()
{
}

////////////////////////////////////////////////////////////////////////////////

XIdBoard::View::View(Model*m, QWidget*parent) : IdTableView(m)
{
}

XIdBoard::View::~View()
{
}

XIdBoard::Model*          XIdBoard::View::model()
{
    return static_cast<Model*>(m_model);
}

const XIdBoard::Model*    XIdBoard::View::model() const
{
    return static_cast<const Model*>(m_model);
}

////////////////////////////////////////////////////////////////////////////////

#include "moc_XIdBoard.cpp"
