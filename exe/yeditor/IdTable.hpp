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
    class IdTableView;
}

using namespace yq;
using namespace yq::mithril;

class IdTable : public Window {
    Q_OBJECT
public:

    ~IdTable();
    
    IdModel*            model() { return m_model; }
    const IdModel*      model() const { return m_model; }
    IdTableView*        view() { return m_view; }
    const IdTableView*  view() const { return m_view; }
    
    void                finalize();
    
public slots:
    void            refresh();
    void            update();
    
private slots:
    void            doubleClicked(const QModelIndex&);

protected:
    IdTable(IdModel*, QWidget* parent=nullptr);

    IdModel*        m_model = nullptr;
    IdTableView*    m_view  = nullptr;
};



template <IdType S>
class IdTableT : public IdTable {
public:

    IdTableT(all_k, QWidget* parent=nullptr) :
        IdTable( new IdModelT<S>(IdModel::Type::Table, ALL), parent )
    {
    }

    IdTableT(all_k, std::span<const ColumnSpec>, QWidget* parent=nullptr) :
        IdTable( new IdModelT<S>(IdModel::Type::Table, ALL), parent )
    {
    }

    IdTableT(std::function<std::vector<S>()> fn, QWidget*parent=nullptr)  : 
        IdTable( new IdModelT<S>(IdModel::Type::Table, IdModelT<S>::toProvider(fn)), parent )
    {
    }

    IdModelT<S>*            model() { return static_cast<IdModelT<S>*>(m_model); }
    const IdModelT<S>*      model() const { return static_cast<IdModelT<S>*>(m_model); }

protected:
    IdTableT(IdModelT<S>* mdl, QWidget*parent) : IdTable(mdl, parent)
    {
    }
};
