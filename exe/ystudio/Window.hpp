////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ystudio.hpp"
#include <gluon/app/SubWindow.hpp>

class Window : public yq::gluon::SubWindow {
    Q_OBJECT
public:

signals:
    void    popupRequested(Id);
    void    openRequested(Id);
    
protected:
    Window(QWidget*parent=nullptr);
    ~Window();
    
};

