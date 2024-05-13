////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "QProject.hpp"

#include <gluon/core/QtTypes.hpp>

namespace yq::doodler {
    QProjectPtr  QProject::load(const QString& fp)
    {
        using namespace yq::gluon;
        QProjectPtr     qpp = new QProject;
        if(qpp -> load_impl(yFilePath(fp)))
            return qpp;
        return {};
    }

    QProject::QProject(QObject*parent) : QObject(parent)
    {
    }
    
    QProject::~QProject()
    {
    }

    QString     QProject::fileName() const
    {
        using namespace yq::gluon;
        return qFilePath(file());
    }

    void    QProject::saveTo(const QString&s) const
    {
        using namespace yq::gluon;
        save_to(yFilePath(s));
    }
    
    void    QProject::saveAs(const QString&s)
    {
        using namespace yq::gluon;
        if(save_as(yFilePath(s)) == std::error_code()){
            emit fileChanged(s);
        }
    }

    void    QProject::setTitle(const QString&str)
    {
        using namespace yq::gluon;
        set_title(yString(str));
        emit titleChanged(str);
    }

    QString     QProject::title() const
    {
        using namespace yq::gluon;
        return qString(Project::title());
    }
}


#include "moc_QProject.cpp"

