////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdTable.hpp>

using namespace yq::mithril;

class RootTable : public IdTableT<Root> {
    Q_OBJECT
public:
    RootTable(QWidget*parent=nullptr);
    ~RootTable();
};
