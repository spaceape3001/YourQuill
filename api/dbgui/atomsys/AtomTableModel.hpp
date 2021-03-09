////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/U64TableModel.hpp>
#include <db/AtomSys.hpp>
#include <dbgui/atomsys/AtomProvider.hpp>


class AtomTableModel : public U64TableModel<Atom> {
    Q_OBJECT
public:
    
    AtomTableModel(AtomProvider ctp=AtomProvider(), QObject*parent=nullptr);
    ~AtomTableModel();
};
