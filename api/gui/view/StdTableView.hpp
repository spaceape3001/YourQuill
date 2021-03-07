////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/delegate/Delegate.hpp>
#include <gui/model/StdTableModel.hpp>
#include <QTableView>

class StdTableView : public QTableView {
protected:
    StdTableView(StdTableModel* m, QWidget* parent=nullptr) : QTableView(parent)
    {
        setModel(m);
        for(auto c : m->columns()){
            Delegate*d  = c->delegate(this);
            if(d)
                setItemDelegateForColumn(c->id(), d);
        }
    }

};
