////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "MetaType.hpp"
#include <util/collection/Set.hpp>
#include <util/collection/Vector.hpp>

namespace yq {
    template <typename T> struct TypedMetaObject;
    class Object;

    class MetaObject : public MetaType {
        template <typename T> friend struct TypedMetaObject;
        friend class MetaProperty;
    public:

        static const Vector<const MetaObject*>&     all();
        static const MetaObject*                    lookup(unsigned int);
        static const MetaObject*                    lookup(const String&);
        
        const auto&         all_derives() const { return m_all.derives; }
        
        /*! \brief  Our "base" class
        */
        const MetaObject*   base() { return m_base; }

        bool                can_cast_to(const MetaObject*) const;
        
        /*! \brief Creates an instance of the class
            \return a pointer to the new object, or NULL if the object is abstract.
        */
        virtual Object*     allocate() const = 0;
        
        /*! \brief Vector of derived classes (not including this one)
        */
        const auto&         derives() const { return m_local.derives; }
        
        /*! \brief TRUE if declared abstract 
        */
        virtual bool        is_abstract() const = 0;
        
        /*! \brief TRUE if the arg is a base to this class
        */
        bool                is_base(const MetaObject*) const;
        
        /*! \brief TRUE if the arg is derived to this class
        */
        bool                is_derived(const MetaObject*) const;
        

        virtual const char* generic() const override { return "MetaObject"; }
        
    protected:
        MetaObject(const char*, const char*);
        
        virtual void            insert() override;
        virtual void            init_me()  = 0;

    private:
        using PropMap   = Map<String,const MetaProperty*,IgCase>;
        using MethodMap = Map<String,const MetaMethod*,IgCase>;

        MetaObject*                     m_base;         //!< Immediate base
        Vector<MetaObject*>             m_bases;        //!< All base classes (from most to least related)
        
        struct D {
            Vector<const MetaObject*>   derives;
            MethodMap                   method;
            StringSet                   methods;
            PropMap                     prop;
            StringSet                   props;
        };
        
        D                           m_all, m_local;
        
        virtual void                inherit();
    };
}
