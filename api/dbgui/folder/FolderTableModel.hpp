////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/U64TableModel.hpp>
#include <db/Folder.hpp>
#include "FolderProvider.hpp"


class FolderTableModel : public U64TableModel<Folder> {
    Q_OBJECT
public:
    
    FolderTableModel(FolderProvider ctp=FolderProvider(), QObject*parent=nullptr);
    ~FolderTableModel();
};
