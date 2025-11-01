////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <map>
#include <mithril/id/Id.hpp>

namespace yq::mithril {
    class IdSignalMapper : public QObject {
        Q_OBJECT
    public:
        IdSignalMapper(QObject* parent=nullptr);
        virtual ~IdSignalMapper ();

        void        setMapping(QObject*, Id);
        void        removeMappings(QObject*);

    signals:
        void        mapped(Id);

    private slots:
        void        map();
        void        remove();

    private:
        std::map<QObject*,Id>    m_mapping;    

        IdSignalMapper(const IdSignalMapper&) = delete;
        IdSignalMapper(IdSignalMapper&&) = delete;
        IdSignalMapper& operator=(const IdSignalMapper&) = delete;
        IdSignalMapper& operator=(IdSignalMapper&&) = delete;
    };
}
