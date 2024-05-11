////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <doodle/Project.hpp>
#include <0/basic/Ref.hpp>

namespace yq::doodle {
    class QProject : public QObject, public Project, public RefCount {
        Q_OBJECT
    public:
    
        QProject(QObject*parent=nullptr);
        virtual ~QProject();
        
        QString     title() const;
        
    signals:
        void    titleChanged(const QString&);
        
    public slots:
        void    setTitle(const QString&);
    };
    
    using QProjectPtr   = Ref<QProject>;
}

