////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/U64TableModel.hpp>
#include <db/AtomSys.hpp>
#include <dbgui/atomsys/ClassProvider.hpp>


class ClassTableModel : public U64TableModel<Class> {
    Q_OBJECT
public:
    
    ClassTableModel(ClassProvider ctp=ClassProvider(), QObject*parent=nullptr);
    ~ClassTableModel();
};
