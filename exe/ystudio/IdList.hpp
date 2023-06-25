////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Window.hpp"
#include <mithrilQt/id/IdModel.hpp>
#include <QWidget>
#include <functional>

namespace yq::mithril {
    class IdListView;
}

using namespace yq;
using namespace yq::mithril;

class IdList : public Window {
    Q_OBJECT
public:

    ~IdList();
    
    IdModel*            model() { return m_model; }
    const IdModel*      model() const { return m_model; }
    IdListView*         view() { return m_view; }
    const IdListView*   view() const { return m_view; }
    
public slots:
    void            refresh();
    void            update();
    
protected:
    IdList(IdModel*, QWidget* parent=nullptr);

    IdModel*        m_model = nullptr;
    IdListView*     m_view  = nullptr;
};



template <IdType S>
class IdListT : public IdList {
public:

    IdListT(all_t, QWidget*parent)  : 
        IdListT(ALL, IdColumn::defaultList<S>(), parent)
    {
    }

    IdListT(all_t, ColumnSpec cs, QWidget*parent)  : 
        IdList( new IdModelT<S>(IdModel::Type::List, ALL), parent )
    {
        m_model -> setColumn(cs);
        m_model -> reload();
    }


    IdListT(std::function<std::vector<S>()> fn, QWidget*parent)  : 
        IdListT(std::move(fn), IdColumn::defaultList<S>(), parent )
    {
    }

    IdListT(std::function<std::vector<S>()> fn, ColumnSpec cs, QWidget*parent)  : 
        IdList( new IdModelT<S>(IdModel::Type::List, IdModelT<S>::toProvider(fn)), parent )
    {
        m_model -> setColumn(cs);
        m_model -> reload();
    }

    IdModelT<S>*            model() { return static_cast<IdModelT<S>*>(m_model); }
    const IdModelT<S>*      model() const { return static_cast<IdModelT<S>*>(m_model); }

protected:
    IdListT(IdModelT<S>* mdl, QWidget*parent) : IdList(mdl, parent)
    {
    }
};
