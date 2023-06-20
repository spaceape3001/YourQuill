////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <QTableView>
#include <functional>

namespace yq::mithril {

    class IdTable : public QWidget {
        Q_OBJECT
    public:
    
        ~IdTable();
        
        void            addColumn(IdColumn&&);
        void            addColumn(size_t before, IdColumn&&);
        
        class Model;
        class View;
        
        IdModel*        model() { return m_model; }
        const IdModel*  model() const { return m_model; }
        View*           view() { return m_view; }
        const View*     view() const { return m_view; }
        
    public slots:
        void            refresh();
        void            update();
        
    protected:
        IdTable(IdModel*, QWidget* parent=nullptr);

        IdModel*    m_model = nullptr;
        View*       m_view  = nullptr;
    };

    
    class IdTable::View : public QTableView {
        Q_OBJECT
    public:
        View(IdModel*, QWidget*parent=nullptr);
        ~View();
    private:
        IdModel*    m_model = nullptr;
    };

    
    template <typename S>
    class IdTableT : public IdTable {
    public:
    
        IdTableT(std::function<std::vector<S>()> fn, QWidget*parent)  : 
            IdTable( new IdModelT<S>(IdModel::Type::Table, IdModelT<S>::toProvider(fn)), parent )
        {
        }
        
        IdTableT(S root, std::function<std::vector<S>(S)> fn, QWidget*parent)  : 
            IdTable( new IdModelT<S>(root, IdModel::Type::Table, IdModelT<S>::toProvider(fn)), parent )
        {
        }
        
        using IdTable::addColumn;
        
        template <typename T>
        void    makeColumn(std::string_view label, std::function<T(S)> fn)
        {
            model()->template makeColumn<T>(label, std::move(fn));
        }
        
        void    setVHeader(std::function<QVariant(S)>fn)
        {
            model()->setVHeader(std::move(fn));
        }
        
        IdModelT<S>*            model() { return static_cast<IdModelT<S>*>(m_model); }
        const IdModelT<S>*      model() const { return static_cast<IdModelT<S>*>(m_model); }

    protected:
        IdTableT(IdModelT<S>* mdl, QWidget*parent) : IdTable(mdl, parent)
        {
        }
    };
}
