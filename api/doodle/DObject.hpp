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

    class DObjectInfo : public ObjectInfo {
    public:
        template <typename C> class Writer;
        
        DObjectInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl=std::source_location::current());
        
        //Object* create() const override final { return nullptr; }
        
        virtual DObject* createD(Project&) const = 0;
        virtual DObject* copyD(Project&, const DObject&) const = 0;
        
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
    };

    template <typename Obj>
    concept DObjectType = std::derived_from<Obj,DObject>;
    

    //! A doodle object, can be associations, constraints, or a physical thing
    class DObject : public Object {
        YQ_OBJECT_INFO(DObjectInfo)
        YQ_OBJECT_FIXER(DObjectFixer)
        YQ_OBJECT_DECLARE(DObject, Object)
        friend class Project;
    public:

        constexpr D             id() const noexcept { return m_id; }
        D                       parent() const noexcept { return m_parent; }
        std::span<const D>      children() const noexcept { return m_children; }
    
        //! Ad-hoc "notes"
        const std::string&      notes() const { return m_notes; }
        void                    set_notes(const std::string&);

        const std::string&      title() const { return m_title; }
        void                    set_title(const std::string&);
        
        void                    bump();
        
        DObject*                create_child(const DObjectInfo&);
        
        template <DObjectType Obj>
        Obj*                    create_child()
        {
            return static_cast<Obj*>(create_child(Obj::staticMetaInfo()));
        }
        
        
    protected:

        //! Remap IDs/pointers appropriately (call base class first)
        virtual void            remap(const Remapper&);

        DObject(Project&);
        DObject(Project&, const DObject&);
        ~DObject();

    private:
        DObject(const DObject&) = delete;
        DObject(DObject&&) = delete;
        DObject& operator=(const DObject&) = delete;
        DObject& operator=(DObject&&) = delete;
    
        Project&                m_project;
        const D                 m_id;
        D                       m_parent;
        std::vector<D>          m_children;
        std::string             m_title;
        std::string             m_notes;
    };
    
    struct Remapper {
        std::unordered_map<D::id_t, D::id_t>    data;
        D operator()(D) const;
    };
    

#if 0    
    
    : public Object {
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
        
        
        
        
    private:
        id_t                        m_parent = 0;
        std::vector<id_t>           m_children;
        std::string                 m_notes;
        
        
        DObject*                    m_parent;
        std::vector<Ref<DObject>>   m_children;
        std::string                 m_notes;
        
        Ref<DObject>            _clone(Remapper&) const;

    };
#endif    

    template <typename C>
    class DObjectInfo::Writer : public ObjectInfo::Writer<C> {
    public:
        Writer(DObjectInfo* dInfo) : ObjectInfo::Writer<C>(dInfo)
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

        
        virtual DObject* createD(Project&prj) const override
        {
            if constexpr (std::is_constructible_v<Obj, Project&> && !std::is_abstract_v<Obj>) {
                if(ObjectInfo::is_abstract())
                    return nullptr;
                return new Obj(prj);
            } else
                return nullptr;
        }
        
        virtual DObject* copyD(Project&prj, const DObject&obj) const override
        {
            if constexpr (std::is_constructible_v<Obj, Project&, const Obj&> && !std::is_abstract_v<Obj>) {
                if(ObjectInfo::is_abstract())
                    return nullptr;
                return new Obj(prj, static_cast<const Obj&>(obj));
            } else
                return nullptr;
        }
    };
}

