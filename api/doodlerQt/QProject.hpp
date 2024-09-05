////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <doodler/Project.hpp>
#include <yq-toolbox/basic/Ref.hpp>

namespace yq::doodler {
    class QProject;

    using QProjectPtr   = Ref<QProject>;

    class QProject : public QObject, public Project, public RefCount {
        Q_OBJECT
    public:
    
        QProject(QObject*parent=nullptr);
        virtual ~QProject();
        
        QString     fileName() const;
        QString     title() const;
        
        static QProjectPtr  load(const QString&);
        
    signals:
        void        titleChanged(const QString&);
        void        fileChanged(const QString&);
        
    public slots:
        void        setTitle(const QString&);
        
        void        saveTo(const QString&) const;
        void        saveAs(const QString&);
        
    };
    
}

