////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/MainWindow.hpp>
#include <doodlerQt/QProject.hpp>

class DoodleMW : public yq::gluon::MainWindow {
    Q_OBJECT
public:
    DoodleMW(yq::doodler::QProjectPtr qpp={});
    ~DoodleMW();
    
    virtual DoodleMW* newMain();
    
private:
    yq::doodler::QProjectPtr      m_project;
};
