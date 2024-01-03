////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/Object.hpp>
#include <0/basic/Ref.hpp>

namespace yq::mithril {
    struct KeyValue;

    /*!  A "Standard Object"
    
        A standard object is one that can serialize itself via key-values.  
    */
    class StdObject : public Object, public RefCount {
        YQ_OBJECT_DECLARE(StdObject, Object)
    public:
    
        /*! Creates the standard object
            
            The standard object load/save routines will assume that the "data" of the root_dir key-value
            represents the class name (from the meta system).

            \param[in] keyvalue Configuration (where "data" is assumed to be type)
            \param[in] bind     Minimum base class expected (will reject otherwise)
        */
        static Ref<StdObject>   load(const KeyValue& keyvalue, const ObjectInfo& base = meta<StdObject>());
        
        template <typename C>
        static Ref<C>           loadAs(const KeyValue& keyvalue)
        {
            Ref<StdObject>      obj = load(keyvalue, meta<C>());
            return Ref<C>(static_cast<C*>(obj.ptr()));
        }

        //! Default will load properties from sub-pairs
        virtual bool    read(const KeyValue&);
        //! Default will pull out properties and save those as sub-pairs
        virtual bool    save(KeyValue&) const;
        
        Ref<MyObject>           clone() const { return clone_me(); }
        virtual StdObject*     clone_me() const = 0;
        
        StdObject();
        virtual ~StdObject();
    };
    
    #define YQ_STD_OBJECT_ABSTRACT(name, par)                                                           \
        YQ_OBJECT_DECLARE(name, par)                                                                    \
        static Ref<MyObject>  load(const KeyValue& keyValue) { return loadAs<MyObject>( keyValue); }    \
        Ref<MyObject>         clone() const { return static_cast<MyObject*>(clone_me()); } 
    
    #define YQ_STD_OBJECT_DECLARE(name, par)        \
        YQ_OBJECT_DECLARE(par)                      \
        virtual name* clone_me() const override;
    
    #define YQ_STD_OBJECT_IMPLEMENT(name)           \
        YQ_OBJECT_IMPLEMENT(name)                   \
        name*  name::clone_me() const { return new MyObject(*this); }
}
