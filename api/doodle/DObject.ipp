////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DObject.hpp"

namespace yq::doodle {

    DObjectInfo::DObjectInfo(std::string_view zName, const MetaObjectInfo& base, const std::source_location& sl) : 
        MetaObjectInfo(zName, base, sl)
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

    DObject::DObject(DObject* parent) : m_parent(parent)
    {
    }
    
    DObject::DObject(const DObject&cp) 
    {
        // nothing to copy
    }

    DObject::~DObject()
    {
        m_children.clear();
        m_parent    = nullptr;
    }

    Ref<DObject> DObject::_clone(Remapper& rmap) const
    {
        Ref<DObject>    ret = metaInfo().createCopyD(*this);
        rmap[id()]  = ret->id();
        for(const DObject* ch : m_children){
            Ref<DObject>    rch = ch -> _clone(rmap);
            assert(rch);
            if(rch){
                rch -> m_parent = ret.ptr();
                ret->m_children.push_back(rch);
            }
        }
        return ret;
    }

    Ref<DObject>    DObject::clone() const
    {
        Remapper        remap;
        Ref<DObject>    ret = _clone(remap);
        ret -> remap(remap);
        return ret;
    }

    void        DObject::remap(const Remapper& theMap)
    {
        for(auto& dobj : m_children){
            assert(dobj && "NULL pointer in child vector detected!");
            dobj -> remap(theMap);
        }
    }
    
    void    DObject::set_notes(const std::string&v)
    {
        m_notes = v;
        changed();
    }

    static void    reg_dobject()
    {
        auto w = writer<DObject>();
        w.property<std::string>("notes", &DObject::notes).setter(&DObject::set_notes);
    }
}

YQ_OBJECT_IMPLEMENT(yq::doodle::DObject)
