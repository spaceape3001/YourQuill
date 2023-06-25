////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdSignalMapper.hpp"

namespace yq::mithril {
    IdSignalMapper::IdSignalMapper(QObject* parent) 
        : QObject(parent)
    {
    }

    IdSignalMapper::~IdSignalMapper()
    {
    }

    void        IdSignalMapper::setMapping(QObject*sender, Id i)
    {
        m_mapping[sender]  = i;
        connect(sender, SIGNAL(destroyed()), this, SLOT(remove()));
    }

    void        IdSignalMapper::removeMappings(QObject*sender)
    {
        m_mapping.erase(sender);
        if(sender)
            sender->disconnect(this);
    }

    void        IdSignalMapper::map()
    {
        QObject*    s   = sender();
        auto i = m_mapping.find(s);
        if(i != m_mapping.end())
            emit mapped(i->second);
    }

    void        IdSignalMapper::remove()
    {
        QObject*    s   = sender();
        m_mapping.erase(s);
    }
}

