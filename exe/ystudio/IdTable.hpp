////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <QWidget>
#include <functional>

namespace yq::mithril {
    class IdTableView;
}

using namespace yq;
using namespace yq::mithril;

class IdTable : public QWidget {
    Q_OBJECT
public:

    ~IdTable();
    
    IdModel*            model() { return m_model; }
    const IdModel*      model() const { return m_model; }
    IdTableView*        view() { return m_view; }
    const IdTableView*  view() const { return m_view; }
    
public slots:
    void            refresh();
    void            update();
    
protected:
    IdTable(IdModel*, QWidget* parent=nullptr);

    IdModel*        m_model = nullptr;
    IdTableView*    m_view  = nullptr;
};



template <IdType S>
class IdTableT : public IdTable {
public:

    IdTableT(all_t, QWidget* parent=nullptr) :
        IdTable( new IdModelT<S>(IdModel::Type::Table, ALL), parent )
    {
    }

    IdTableT(all_t, std::span<const ColumnSpec>, QWidget* parent=nullptr) :
        IdTable( new IdModelT<S>(IdModel::Type::Table, ALL), parent )
    {
    }

    IdTableT(std::function<std::vector<S>()> fn, QWidget*parent)  : 
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
