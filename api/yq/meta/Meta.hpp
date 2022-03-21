////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/preamble.hpp>
#include <yq/collection/Hash.hpp>
#include <yq/collection/Map.hpp>
#include <yq/collection/MultiMap.hpp>
#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>
#include "VariantDef.hpp"

/*
    META is about runtime reflection & introspection
    
    OBJECT      -- something that has it baked in, blind copying is NOT advised?
    VALUE       -- something that doesn't have it baked in, so copying is normal?
    PROPERTY    -- something that a user can interact with, read/set, on an object/value
    METHOD      -- function (on modifier) on OBJECT or VALUE -- can be a static method (no implicit class/type parameter)
    FUNCTION    -- global method (combinding with method... parent will be "GLOBAL" with global meta object
    GLOBAL      -- global property -- combined with property
    TYPE        -- can have property/methods (even values can, in theory)
    PAGE        -- for the server
    POINTER     -- pointer/ref, assumed to be shared_ptr<T>
    ENUM        -- enumeration (implicitly value)
    FLAG        -- combination of enums (implicitly value)
    
    Also, we don't want to be duplicating data that's already in the program string table, therefore
    all "const char[]" are expected to be pre-baked constants (not from std::string's c_str())
*/


namespace yq {
    //class Variant;
    
    /*
        We want flags, however, they also need to expandable in the other libraries. 
        These flags are intended to avoid needless dynamic casts, check & cast.  
        
        Do NOT rely on the numeric values, those are subject to change.
    */
    enum {
        TYPE        = 1U << 0,
        OBJECT      = 1U << 1,
        COMPOUND    = 1U << 2,
        PROPERTY    = 1U << 3,
        METHOD      = 1U << 4,
        ENUM        = 1U << 5,
        ARG         = 1U << 6,
        GLOBAL      = 1U << 7,
        COLLECTION  = 1U << 8,
        TEMPLATE    = 1U << 9,
        
        
        STATE       = 1U << 20, //!< "State" property
        STATIC      = 1U << 21, //!< Non-object specific (global variables, functions, etc)
        SMALL       = 1U << 22, //!< Small enough to fit in DataBlock
        TLS         = 1U << 23, //!< Thread local storage
        LESS        = 1U << 24  //!< Can compare
    };
    
    //bool    is_type(const Meta&);
    //bool    is_object(const Meta&);
    //bool    is_value(const Meta&);
    //bool    is_global(const Meta&);
    //bool    is_property(const Meta&);
    //bool    is_method(const Meta&);
    //bool    is_function(const Meta&);
    //bool    is_pointer(const Meta&); 
    //bool    is_structure(const Meta&);   

    /* !\brief Root "meta" for all meta, everything else will derive from this.
    
        \note We will ASSUME SINGLE THREADED until AFTER freeze is called, so that no mutex locking is required.
    */
    class Meta {
    public:
    
    
            //  I mean, really, 255 ain't enough?? Even in full-on UTF-8, that's over 40 characters.
            //  This applies to tags  too BTW
        static constexpr const size_t   MAX_NAME            = 255;
        
        static constexpr const size_t   MAX_KEY             = 255;

            //  Label's a bit longer to permit formatting
        static constexpr const size_t   MAX_LABEL           = 1023;
        
            //! No manifestos disguised as descriptions please
        static constexpr const size_t   MAX_DESCRIPTION     = 4095;
        
    
        //! Meta ID type
        using id_t                      = unsigned int;
        
        //! Our flags
        using options_t                 = uint32_t;
        
        static const Vector<const Meta*>&   all();
        static const Meta*                  lookup(id_t);
        
        //! Forces the initialization of the meta
        static void                         init();     // starts/triggers the initialization (can be repeated)
        //! Locks/freezes the meta system (init is called a final time)
        static void                         freeze();
        
        //! Rather than using mutexes, we assert/check the read is being done thread-safe & acceptable
        static bool                         thread_safe_read();
        //! Rather than using mutexes, we assert/check the write is being done thread-safe & acceptable
        static bool                         thread_safe_write();
        
        //! TRUE if we've not been frozen (modifications are rejected after this)
        static bool                         unlocked();


        /*! \brief Any relevant aliases
        */
        const Set<std::string_view>&    aliases() const { return m_aliases; }
        
        //! \brief Vector of child-meta (could include pointers)
        const Vector<const Meta*>&      children() const { return m_children; }
        
        const std::string_view&         description() const { return m_description; }
        
        //! \brief Flags for this class
        uint64_t                        flags() const { return m_flags; }
        
        //! Generic type of this item
        virtual const char*             generic() const { return "Meta"; }

        //  TODO
        bool                            has_tag(const std::string_view&) const;

        //! \brief Our ID number
        id_t                            id() const { return m_id; }
        
        bool                            is_object() const { return (m_flags & OBJECT) != 0; }
        bool                            is_type() const { return (m_flags & TYPE) != 0; }
        
        const std::string_view&         label() const { return m_label; }

        const std::string_view&         name() const { return m_name; }
        
        //  MAY BE NULL
        const Meta*                     parent() const { return m_parent; }
        
        
        //  TODO
        const Variant&                  tag(const std::string_view&) const;
        

            // used during the creation....
        class Writer;
        
        template <typename T>
        struct LUC;

    protected:
        friend class ArgInfo;
        friend class CompoundInfo;
        friend class GlobalInfo;
        friend class MethodInfo;
        friend class ObjectInfo;
        friend class PropertyInfo;
        friend class TypeInfo;
    
        //! Used by the constructor's default to auto-assign an ID
        static constexpr const id_t AUTO_ID = ~(id_t) 0;
        explicit Meta(std::string_view, id_t i=AUTO_ID);
        explicit Meta(std::string_view, Meta* parent, id_t i=AUTO_ID);
        virtual ~Meta();    // do NOT delete... C++ won't let it be deleted
        
            // called after each init... repopulate property maps/vectors/etc (do not append, rebuild)
        void                            sweep();
        virtual void                    sweep_impl() {}
        static void                     sweep_all();
        
        
        void    set_option(options_t v) { m_flags |= v; }
        void    set_options(options_t v) { m_flags |= v; }
        
    private:
        using TagMap    = Map<std::string_view, Variant, IgCase>;
    
        TagMap                          m_tags;
        Set<std::string_view>           m_aliases;
        std::string_view                m_description;
        std::string_view                m_label;
        std::string_view                m_name;
        Vector<const Meta*>             m_children;
        const Meta*                     m_parent    = nullptr;
        options_t                       m_flags     = {};
        id_t                            m_id        = AUTO_ID;
        
        enum {
            SWEPT                   = 1ULL << 31
        };
    };

    template <typename T>
    struct Meta::LUC {
        //   CONDITION is still valid, however, we can't use it w/o compiler issues
        //static_assert( std::is_base_of_v<Meta, T>, "T must derive from Meta!");
        using MM    = MultiMap<std::string_view, const T*, IgCase>;
        Vector<const T*>            all;
        MM                          lut;
        StringViewSet               keys;
        
        LUC& operator<<(const T* p)
        {
            all << p;
            lut.insert(p->name(), p);
            keys << p->name();
            return *this;
        }
        
        LUC&    operator += (const LUC& b)
        {
            all += b.all;
            lut += b.lut;
            keys += b.keys;
            return *this;
        }
    };
}

