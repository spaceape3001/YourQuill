////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/delegate/Delegate.hpp>
#include <gui/model/StdListModel.hpp>
#include <QListView>

template <typename T>
class StdListView : public QListView {
protected:
    StdListView(StdListModel<T>* m, QWidget* parent=nullptr) : QListView(parent)
    {
        setModel(m);
        const StdColumn<T>* c   = m->column();
        if(c){
            Delegate*d  = c->delegate(this);
            if(d)
                setItemDelegate(d);
        }
    }

};
