////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodle/Project.hpp>

namespace yq::doodle {
    Project::Project() : m_revision{0ULL}
    {
        m_objects.push_back(nullptr);
    }
    
    
    Project::~Project()
    {
    }

    D    Project::_insert(DObject*obj)
    {
        D    n   = {(id_t) m_objects.size()};
        m_objects.push_back(obj);
        return n;
    }

    void                Project::bump()
    {
        ++m_revision;
    }

    DObject*            Project::object(D i)
    {
        if((size_t) i.id >= m_objects.size())
            return nullptr;
        return m_objects[i.id];
    }
    
    const DObject*      Project::object(D  i) const
    {
        if((size_t) i.id >= m_objects.size())
            return nullptr;
        return m_objects[i.id];
    }

    uint64_t            Project::revision() const
    {
        return m_revision;
    }
    
    void                Project::set_title(const std::string&v)
    {
        m_title = v;
        bump();
    }

}
