////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/view/StdTableView.hpp>

class Document;
class DocumentTableModel;

class DocumentTableView : public StdTableView<Document> {
    Q_OBJECT
public:
    DocumentTableView(DocumentTableModel*, QWidget*parent=nullptr);
    ~DocumentTableView();
    
    DocumentTableModel*    model() const { return m_model; }
    
private:
    DocumentTableModel*    m_model;
};
