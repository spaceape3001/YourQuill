////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

    // I'd prefer it otherwise, but MOC's messing up
#include <gui/model/U64TableModel.hpp>
#include <db/FileSys.hpp>
#include <dbgui/filesys/DocumentProvider.hpp>


class DocumentTableModel : public U64TableModel<Document> {
    Q_OBJECT
public:
    
    DocumentTableModel(DocumentProvider ctp=DocumentProvider(), QObject*parent=nullptr);
    ~DocumentTableModel();
};
