////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DoodleMW.hpp"

DoodleMW::DoodleMW(yq::doodler::QProjectPtr qpp) : m_project(qpp)
{
    if(!m_project)
        m_project   = new yq::doodler::QProject;
        
        
    makeMenu("file", "&File");
    makeMenu("edit", "&Edit");
    makeMenu("view", "&View");
    makeMenu("doodle", "&Doodle");
        
    enableDetachableTabs();
    setWindowTitle(tr("Doodler"));
    resize(1920, 1080);
}

DoodleMW::~DoodleMW()
{
}

DoodleMW* DoodleMW::newMain()
{
    return new DoodleMW(m_project);
}


#include "moc_DoodleMW.cpp"

