////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/MetaObject.hpp>
#include <doodle/preamble.hpp>

namespace yq::doodle {
    class DObject;
    template <typename Obj> class DObjectFixer;

    class DObjectInfo : public MetaObjectInfo {
    public:
        template <typename C> class Writer;
        
        DObjectInfo(std::string_view zName, const MetaObjectInfo& base, const std::source_location& sl=std::source_location::current());
        
        virtual DObject* createD(DObject* parent=nullptr) const = 0;
        
        bool    is_0d() const;
        bool    is_1d() const;
        bool    is_2d() const;
        bool    is_3d() const;
        bool    is_4d() const;
        bool    is_5d() const;
        bool    is_6d() const;
        bool    is_association() const;
        bool    is_camera() const;
        bool    is_constraint() const;
        bool    is_light() const;
        bool    is_motion() const;
        bool    is_physical() const;
        bool    is_space() const;
        
    private:
        friend class DObject;
        virtual DObject* createCopyD(const DObject&) const = 0;
    };

    //! A doodle object, can be associations, constraints, or a physical thing
    class DObject : public MetaObject {
        YQ_OBJECT_INFO(DObjectInfo)
        YQ_OBJECT_FIXER(DObjectFixer)
        YQ_OBJECT_DECLARE(DObject, MetaObject)
        template <typename> friend class Ref;
    public:
    
        //! Deep copy of the object, the preferred way
        Ref<DObject>        clone() const;
        
        //! Ad-hoc "notes"
        const std::string&  notes() const { return m_notes; }
        void                set_notes(const std::string&);
    
    protected:
        DObject(DObject* parent=nullptr);
        //! Copies this object but does *NOT* deep copy the children
        DObject(const DObject&);
        ~DObject();
        
        
        //! Remap IDs/pointers appropriately (call base class first)
        virtual void        remap(const Remapper&) = 0;
        
        
    private:
        DObject*                    m_parent;
        std::vector<Ref<DObject>>   m_children;
        std::string                 m_notes;
        
        Ref<DObject>            _clone(Remapper&) const;

        DObject(DObject&&) = delete;
        DObject& operator=(const DObject&) = delete;
        DObject& operator=(DObject&&) = delete;
    };
    

    template <typename C>
    class DObjectInfo::Writer : public MetaObjectInfo::Writer<C> {
    public:
        Writer(DObjectInfo* dInfo) : MetaObjectInfo::Writer<C>(dInfo)
        {
        }
        
        Writer(DObjectInfo& dInfo) : Writer(&dInfo)
        {
        }
    };
    
    template <typename Obj>
    class DObjectFixer : public ObjectFixer<Obj> {
    public:
    
        DObjectFixer(std::string_view szName, typename Obj::MyBase::MyInfo& myBase, std::source_location sl=std::source_location::current()) : 
            ObjectFixer<Obj>(szName, myBase, sl) 
        {
        }

        
        virtual DObject* createD(DObject* parent) const
        {
            if constexpr (std::is_constructible_v<Obj, Obj*> && !std::is_abstract_v<Obj>) {
                if(ObjectInfo::is_abstract())
                    return nullptr;
                return new Obj(parent);
            } else
                return nullptr;
        }

        virtual DObject*    createCopyD(const DObject& cp) const override
        {
            if constexpr (std::is_copy_constructible_v<Obj> && !std::is_abstract_v<Obj>) {
                if(ObjectInfo::is_abstract())
                    return nullptr;
                return new Obj(cp);
            } else
                return nullptr;
        }
    };
}

