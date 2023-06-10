////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/MainWindow.hpp>

class DreamMW : public yq::gluon::MainWindow {
    Q_OBJECT
public:

    DreamMW();
    ~DreamMW();
    
    
    yq::gluon::MainWindow*   newMain() override;
    
public slots:
    void    updateTitle();
    
    void    newBrowser();
};
