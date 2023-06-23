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
    std::vector<Id> empty_provider()
    {
        return std::vector<Id>();
    }

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
    m_model = new Model();
    m_view  = new View(m_model);
    setWidget(m_view);
    
    setWindowTitle("Clipboard");
}

XClipboard::~XClipboard()
{
}


////////////////////////////////////////////////////////////////////////////////

XClipboard::Model::Model(QObject* parent) : IdModel(Type::Table, Id(), empty_provider)
{
    m_enableReload  = false;
    addColumn(column::id_key(ColOpt::Icon));
    addColumn(column::id_label());
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
