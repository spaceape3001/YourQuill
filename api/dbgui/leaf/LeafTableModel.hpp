////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/U64TableModel.hpp>
#include <db/Leaf.hpp>
#include <dbgui/leaf/LeafProvider.hpp>


class LeafTableModel : public U64TableModel<Leaf> {
    Q_OBJECT
public:
    
    LeafTableModel(LeafProvider ctp=LeafProvider(), QObject*parent=nullptr);
    ~LeafTableModel();
};
