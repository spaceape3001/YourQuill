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
    
        IdTable(Id, IdProvider&&, QWidget* parent=nullptr);
        IdTable(IdProvider&&, QWidget*parent=nullptr);
        ~IdTable();
        
        void            addColumn(IdColumn&&);
        void            addColumn(size_t before, IdColumn&&);
        
        class Model;
        class View;
        
        Model*          model() { return m_model; }
        const Model*    model() const { return m_model; }
        View*           view() { return m_view; }
        const View*     view() const { return m_view; }
        
    public slots:
        void            refresh();
        void            update();
        
    protected:
        Model*  m_model = nullptr;
        View*   m_view  = nullptr;
    };


    class IdTable::Model : public IdModel {
        Q_OBJECT
    public:
        Model(Id, IdProvider&&, QObject*parent=nullptr);
        ~Model();
    };
    
    class IdTable::View : public QTableView {
        Q_OBJECT
    public:
        View(Model*, QWidget*parent=nullptr);
        ~View();
    private:
        Model*  m_model = nullptr;
    };

    
    template <typename S>
    class IdTableT : public IdTable {
    public:
    
        IdTableT(std::function<std::vector<S>()> fn, QWidget*parent)  :
            IdTable([fn]()->std::vector<Id>{
                std::vector<S>  data    = fn();
                return std::vector<Id>(data.begin(), data.end());
            }, parent)
        {
        }
        
        IdTableT(S root, std::function<std::vector<S>(S)> fn, QWidget*parent) : 
            IdTable(root, [fn,root]()->std::vector<Id>{
                std::vector<S> data = fn(root);
                return std::vector<Id>(data.begin(), data.end());
            }, parent)
        {
        }
        
        using IdTable::addColumn;
        
        template <typename T>
        void    makeColumn(std::string_view label, std::function<T(S)> fn)
        {
            m_model->addColumn<S,T>(label, fn);
        }
    };
}
