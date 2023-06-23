////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdTreeView.hpp>

class XFiles : public yq::mithril::IdTreeView {
    Q_OBJECT
public:    

    class Model;


    XFiles(QWidget*parent=nullptr);
    ~XFiles();
    
    Model*          model();
    const Model*    model() const;
};

