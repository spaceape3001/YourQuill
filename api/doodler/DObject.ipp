////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObject.hpp>
#include <doodler/Project.hpp>
//#include <0/basic/CollectionUtils.hpp>
#include <yq-toolbox/container/map_utils.hpp>

namespace yq::doodler {

    DObjectInfo::DObjectInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl) : 
        ObjectInfo(zName, base, sl)
    {
        set(Flag::DOODLE);
    }


    bool    DObjectInfo::is_0d() const
    {
        return has(Flag::D0);
    }
    
    bool    DObjectInfo::is_1d() const
    {
        return has(Flag::D1);
    }
    
    bool    DObjectInfo::is_2d() const
    {
        return has(Flag::D2);
    }
    
    bool    DObjectInfo::is_3d() const
    {
        return has(Flag::D3);
    }
    
    bool    DObjectInfo::is_4d() const
    {
        return has(Flag::D4);
    }
    
    bool    DObjectInfo::is_5d() const
    {
        return has(Flag::D5);
    }
    
    bool    DObjectInfo::is_6d() const
    {
        return has(Flag::D6);
    }
    
    bool    DObjectInfo::is_association() const
    {
        return has(Flag::ASSOCIATION);
    }

    bool    DObjectInfo::is_camera() const
    {
        return has(Flag::CAMERA);
    }

    bool    DObjectInfo::is_constraint() const
    {
        return has(Flag::CONSTRAINT);
    }

    bool    DObjectInfo::is_light() const
    {
        return has(Flag::LIGHT);
    }
    
    bool    DObjectInfo::is_motion() const
    {
        return has(Flag::MOTION);
    }
    
    bool    DObjectInfo::is_physical() const
    {
        return has(Flag::PHYSICAL);
    }
    
    bool    DObjectInfo::is_space() const
    {
        return has(Flag::SPACE);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    DObject::DObject(Project&prj) : m_project(prj), m_id(prj._insert(this))
    {
    }
    
    
    DObject::DObject(Project&prj, const DObject&cp) : DObject(prj)
    {
        m_parent    = cp.m_parent;
        m_children  = cp.m_children;
        m_title     = cp.m_title;
        m_notes     = cp.m_notes;
    }

    DObject::~DObject()
    {
        m_children.clear();
        m_parent    = {};
    }

    std::string DObject::attribute(const std::string& k) const
    {
        return get_value(m_attributes, k);
    }

    void    DObject::bump()
    {
        m_project.bump();
    }

    DObject*   DObject::create_child(const DObjectInfo& info)
    {
        DObject*  newD    = info.createD(m_project);
        if(!newD)
            return nullptr;
        m_children.push_back(newD->id());
        return newD;
    }

    //Ref<DObject> DObject::_clone(Remapper& rmap) const
    //{
        //Ref<DObject>    ret = metaInfo().createCopyD(*this);
        //rmap[id()]  = ret->id();
        //for(const DObject* ch : m_children){
            //Ref<DObject>    rch = ch -> _clone(rmap);
            //assert(rch);
            //if(rch){
                //rch -> m_parent = ret.ptr();
                //ret->m_children.push_back(rch);
            //}
        //}
        //return ret;
    //}

    //Ref<DObject>    DObject::clone() const
    //{
        //Remapper        remap;
        //Ref<DObject>    ret = _clone(remap);
        //ret -> remap(remap);
        //return ret;
    //}

    void        DObject::remap(const Remapper& theMap)
    {
        m_parent    = theMap(m_parent);
        
        for(ID& i : m_children){
            i   = theMap(i);
            DObject*    dob = m_project.object(i);
            if(dob)
                dob -> remap(theMap);
        }
    }
    
    void    DObject::set_attribute(const std::string& key, const std::string& value)
    {
        m_attributes[key]   = value;
        bump();
    }

    void    DObject::set_notes(const std::string&v)
    {
        m_notes = v;
        bump();
    }

    void    DObject::set_title(const std::string&v)
    {
        m_title = v;
        bump();
    }

    void    DObject::set_uid(const std::string& v)
    {
        m_uid       = v;
        bump();
    }


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    
    ID Remapper::operator()(ID i) const
    {
        auto j = data.find(i.id);
        if(j != data.end())
            return { j->second };
        return i;
    }
}

