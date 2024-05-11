////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "QProject.hpp"

#include <gluon/core/QtTypes.hpp>

namespace yq::doodle {
    QProject::QProject(QObject*parent) : QObject(parent)
    {
    }
    
    QProject::~QProject()
    {
    }

    QString     QProject::title() const
    {
        using namespace yq::gluon;
        return qString(Project::title());
    }

    void    QProject::setTitle(const QString&str)
    {
        using namespace yq::gluon;
        set_title(yString(str));
        emit titleChanged(str);
    }
}


#include "moc_QProject.cpp"

