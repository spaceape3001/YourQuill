////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gluon/app/MainWindow.hpp>
#include <doodleQt/QProject.hpp>

class DoodleMW : public yq::gluon::MainWindow {
    Q_OBJECT
public:
    DoodleMW(yq::doodle::QProjectPtr qpp={});
    ~DoodleMW();
    
    virtual DoodleMW* newMain();
    
private:
    yq::doodle::QProjectPtr      m_project;
};
