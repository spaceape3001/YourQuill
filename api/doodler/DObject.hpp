////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/core/Flags.hpp>
#include <yq/core/MetaObject.hpp>
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
        
        DObjectInfo(std::string_view zName, ObjectInfo& base, const std::source_location& sl=std::source_location::current());
        
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
    concept SomeDObject = std::derived_from<Obj,DObject>;
    

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
        
        template <SomeDObject Obj>
        Obj*                    create_child()
        {
            return static_cast<Obj*>(create_child(Obj::staticMetaInfo()));
        }
        
        
        static void init_info();
        
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

}

