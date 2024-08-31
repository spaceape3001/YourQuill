////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/basic/Flags.hpp>
#include <0/basic/MetaObject.hpp>
#include <doodler/preamble.hpp>

namespace yq::doodler {
    class DObject;
    template <typename Obj> class DObjectFixer;
    
    enum class Dim : uint8_t {
        D0  = 0,
        D1,
        D2,
        D3,
        D4,
        D5,
        D6
    };
    
    using DimFlags = Flags<Dim, uint8_t>;
    

    class DObjectInfo : public ObjectInfo {
    public:
        template <typename C> class Writer;
        
        DObjectInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl=std::source_location::current());
        
        //Object* create() const override final { return nullptr; }
        
        virtual DObject* createD(Project&) const = 0;
        virtual DObject* copyD(Project&, const DObject&) const = 0;
        
        DimFlags    supports() const { return m_supports; }
        
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
        
    protected:
        DimFlags    m_supports;
    };

    template <typename Obj>
    concept DObjectType = std::derived_from<Obj,DObject>;
    

    /*! \brief A doodle object
    
        Attributes are user-defined (and 
        may be needed by end designs)
    */
    class DObject : public Object {
        YQ_OBJECT_INFO(DObjectInfo)
        YQ_OBJECT_FIXER(DObjectFixer)
        YQ_OBJECT_DECLARE(DObject, Object)
        friend class Project;
    public:

        constexpr ID            id() const noexcept { return m_id; }
        ID                      parent() const noexcept { return m_parent; }
        std::span<const ID>     children() const noexcept { return m_children; }
    
        //! Ad-hoc "notes"
        const std::string&      notes() const { return m_notes; }
        void                    set_notes(const std::string&);

        const std::string&      title() const { return m_title; }
        void                    set_title(const std::string&);
        
        const string_map_t&     attributes() const { return m_attributes; }
        
        void                    set_attribute(const std::string& key, const std::string& value);
        std::string             attribute(const std::string&) const;
        
        
        const std::string&      uid() const { return m_uid; }
        void                    set_uid(const std::string&);
        
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
        const ID                m_id;
        ID                      m_parent;
        std::vector<ID>         m_children;
        std::string             m_title;
        std::string             m_uid;
        std::string             m_notes;
        string_map_t            m_attributes;
    };
    
    struct Remapper {
        std::unordered_map<ID::id_t, ID::id_t>    data;
        ID operator()(ID) const;
    };
    
    template <typename C>
    class DObjectInfo::Writer : public ObjectInfo::Writer<C> {
    public:
        Writer(DObjectInfo* dInfo) : ObjectInfo::Writer<C>(dInfo), m_meta(dInfo)
        {
        }
        
        Writer(DObjectInfo& dInfo) : Writer(&dInfo)
        {
        }
        
        Writer& support_0d()
        {
            if(m_meta)
                m_meta -> m_supports |= Dim::D0;
            return *this;
        }
        
        Writer& support_1d()
        {
            if(m_meta)
                m_meta -> m_supports |= Dim::D1;
            return *this;
        }

        Writer& support_2d()
        {
            if(m_meta)
                m_meta -> m_supports |= Dim::D2;
            return *this;
        }

        Writer& support_3d()
        {
            if(m_meta)
                m_meta -> m_supports |= Dim::D3;
            return *this;
        }

        Writer& support_4d()
        {
            if(m_meta)
                m_meta -> m_supports |= Dim::D4;
            return *this;
        }

        Writer& support_5d()
        {
            if(m_meta)
                m_meta -> m_supports |= Dim::D5;
            return *this;
        }

        Writer& support_6d()
        {
            if(m_meta)
                m_meta -> m_supports |= Dim::D6;
            return *this;
        }
    private:
        DObjectInfo*    m_meta;
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

