////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Dock.hpp"

class XFiles : public Dock {
    Q_OBJECT
    YQ_OBJECT_DECLARE(XFiles, Dock)
public:    

    class Model;
    class View;

    XFiles(QWidget*parent=nullptr);
    ~XFiles();
    
    Model*          model() { return m_model; }
    const Model*    model() const { return m_model; }
    
    View*           view() { return m_view; }
    const View*     view() const { return m_view; }
    
private:
    Model*      m_model = nullptr;
    View*       m_view  = nullptr;
};

