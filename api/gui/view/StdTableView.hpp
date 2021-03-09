////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/delegate/Delegate.hpp>
#include <gui/model/StdTableModel.hpp>
#include <QTableView>

template <typename T>
class StdTableView : public QTableView {
protected:
    StdTableView(StdTableModel<T>* m, QWidget* parent=nullptr) : QTableView(parent)
    {
        setModel(m);
        for(auto c : m->columns()){
            Delegate*d  = c->delegate(this);
            if(d)
                setItemDelegateForColumn(c->id(), d);
        }
        setSortingEnabled(true);
    }

};
