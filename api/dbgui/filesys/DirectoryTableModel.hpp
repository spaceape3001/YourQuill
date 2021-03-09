////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/U64TableModel.hpp>
#include <db/FileSys.hpp>
#include <dbgui/filesys/DirectoryProvider.hpp>


class DirectoryTableModel : public U64TableModel<Directory> {
    Q_OBJECT
public:
    
    DirectoryTableModel(DirectoryProvider ctp=DirectoryProvider(), QObject*parent=nullptr);
    ~DirectoryTableModel();
};
