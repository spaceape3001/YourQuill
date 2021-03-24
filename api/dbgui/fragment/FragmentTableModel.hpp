////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/U64TableModel.hpp>
#include <db/Fragment.hpp>
#include "FragmentProvider.hpp"


class FragmentTableModel : public U64TableModel<Fragment> {
    Q_OBJECT
public:
    
    FragmentTableModel(FragmentProvider ctp=FragmentProvider(), QObject*parent=nullptr);
    ~FragmentTableModel();
};
