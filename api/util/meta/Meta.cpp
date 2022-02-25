////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Meta.hpp"
#include "Binder.hpp"
#include "Global.hpp"
#include "Internal.hpp"
#include "MetaObject.hpp"
#include "ObjectInfo.hpp"
#include "TypeInfo.hpp"
#include "Init.hpp"


#include <util/app/DelayInit.hpp>
#include <util/stream/Ops.hpp>
#include <string.h>

namespace yq {
void    foo()
{
    std::unordered_map<int, std::string>    data;
    data.find(0) != data.end();
}
}


#include <util/type/Variant.hpp>

namespace yq {


    namespace {
        static constexpr const unsigned int         kReserve        = 8192;     // adjust as necessary

        template <typename T>
        void    set_and_reserve(Vector<T>& vec, size_t idx, size_t inc, const T& val, const T& def)
        {
            if(vec.size() <= idx){
                if(vec.capacity() <= idx)
                    vec.reserve(idx + inc);
                vec.resize(idx+1, def);
            }
            vec[idx]    = val;
        }

        struct Repo {
            bool                    openReg     = true;
            Vector<const Meta*>     all;
            Vector<const Meta*>     metas;
            Meta::LUC<ObjectInfo>   objects;
            Meta::LUC<TypeInfo>     types;
            
            Repo()
            {
                all.reserve(8192);      // adjust as necessary, performance tradeoff, basically
                all.resize(M_USER, nullptr);
            }
        };
        
        Repo&   repo()
        {
            static Repo*    s_repo  = new Repo;
            return *s_repo;
        }
        
        #if 0
        bool    is_open()
        {
            return repo().openReg;
        }
        #endif
        
        const char*    str_start(const char* z, const char* pat)
        {
            const char *y   = z;
            for(; *y && *pat; ++y, ++pat)
                if(*y != *pat)
                    return z;
            return y;
        }
    }
    
    struct EmptyType : public TypeInfo {
    
        EmptyType(id_t i) : TypeInfo( i ? "Any" : "Void", __FILE__, i) 
        {
            m_copyB     = [](DataBlock&, const DataBlock&) {};
            m_copyR     = [](DataBlock&, const void*) {};
            m_ctorCopyR = [](DataBlock&, const void*) {};
            m_ctorCopyB = [](DataBlock&, const DataBlock&) {};
            m_ctorMove  = [](DataBlock&, DataBlock&&) {};
            m_dtor      = [](DataBlock&) {};
            m_equal     = [](const DataBlock&, const DataBlock&) -> bool { return true; };
            m_moveB     = [](DataBlock&, DataBlock&&) {};
            if(i)
                m_print = [](Stream& s, const void*) { s << "(wildcard)"; };
            else
                m_print = [](Stream& s, const void*) { s << "(empty)"; };
        }
        
        ~EmptyType(){}
    };
        
    const TypeInfo&        invalid()
    {
        static EmptyType*  s_ret = new EmptyType(MT_Invalid);
        return *s_ret;
    }
    
    const TypeInfo&        variant()
    {
        static EmptyType*  s_ret = new EmptyType(MT_Variant);
        return *s_ret;
    }




    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  ARG
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

        ArgInfo::ArgInfo(const char* zName, const TypeInfo&t, Meta*par) : Meta(zName, par), m_type(t) 
        {
            m_flags |= ARG;
        }
        
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  COMPOUND
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        CompoundInfo::CompoundInfo(const char zName[], const char zFile[], Meta* par, id_t i) : Meta(zName, par, i), m_file(zFile)
        {
            m_flags |= COMPOUND;
        }
        
        CompoundInfo::~CompoundInfo()
        {
        }

        void        CompoundInfo::sweep_impl() 
        {
            Meta::sweep_impl();
        }

        void        CompoundInfo::gather(LUC<MethodInfo>& res)
        {
            res     = {};
            for(const Meta* m : children()){
                if(m && (m->flags() & METHOD))
                    res << static_cast<const MethodInfo*>(m);
            }
        }
        
        void        CompoundInfo::gather(LUC<PropertyInfo>& res)
        {
            res     = {};
            for(const Meta* m : children()){
                if(m && (m->flags() & PROPERTY))
                    res << static_cast<const PropertyInfo*>(m);
            }
        }
        

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  GLOBAL
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////


        GlobalInfo&     GlobalInfo::instance()
        {
            static GlobalInfo*  s_ret   = new GlobalInfo;
            return *s_ret;
        }

        GlobalInfo::GlobalInfo() : CompoundInfo("Global", __FILE__, nullptr, MC_Global)
        {
        }

        void     GlobalInfo::sweep_impl() 
        {   
            CompoundInfo::sweep_impl();
            gather(m_properties);
            gather(m_methods);
            for(const Meta* m : m_methods.all)
                const_cast<Meta*>(m) -> m_flags |= GLOBAL | STATIC;
            for(const Meta* m : m_properties.all)
                const_cast<Meta*>(m) -> m_flags |= GLOBAL | STATIC;
        }
        
        
        const GlobalInfo&        InfoBinder<Global>::bind()
        {
            return GlobalInfo::instance();
        }
        
        GlobalInfo::Writer       InfoBinder<Global>::edit()
        {
            return GlobalInfo::Writer( &GlobalInfo::instance() );
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  META
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////


        const Vector<const Meta*>&   Meta::all()
        {
            assert(thread_safe_read());
            return repo().all;
        }


        void    Meta::freeze()
        {
            init();
            repo().openReg  = false;
        }

        void    Meta::init()
        {
            if(thread_safe_write()){
                DelayInit::init_all();
                sweep_all();
            }
        }

        void    Meta::sweep_all()
        {
            if(thread_safe_write()){
                Repo&   r = repo();
                for(const Meta* m : r.all)
                    if(m)   // gaps can show
                        const_cast<Meta*>(m) -> m_flags &= ~SWEPT;
                for(const Meta* m : r.all)
                    if(m)
                        const_cast<Meta*>(m) -> sweep();
            }
        }

        bool    Meta::thread_safe_read()
        {
            return (!repo().openReg) || (!thread::id());
        }

        bool    Meta::thread_safe_write()
        {
            return repo().openReg && !thread::id();
        }

        bool    Meta::unlocked()
        {
            return repo().openReg;
        }


        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        Meta::Meta(const char zName[], id_t i) : Meta(zName, nullptr, i)
        {
        }
        
        Meta::Meta(const char zName[], Meta* parent, id_t i) 
        {
            assert(zName);
            assert(strnlen(zName, MAX_NAME+1) <= MAX_NAME);
            assert(thread_safe_write());
            
            //  strip out the yq namespace
            m_name      = str_start(zName, "yq::");
            m_label     = m_name;                       // default (can be overriden)
            m_parent    = parent;

            auto& _r     = repo();
            if(i < M_USER){
                m_id    = i;
                _r.all[i]   = this;
            } else {
                m_id    = _r.all.size();
                _r.all << this;
            }
            if(i >= MT_String)
                _r.metas << this;
        }
        
        Meta::~Meta()
        {
            assert(false && "Do NOT delete Meta!  It's not meant to be deleted.");

        }

        bool  Meta::has_tag(const std::string_view&k) const
        {
            return m_tags.contains(k);
        }

        void  Meta::sweep()
        {
            assert(thread_safe_write());

            if(!(m_flags & SWEPT)){
                sweep_impl();
                m_flags |= SWEPT;
            }
        }

        const Variant&  Meta::tag(const std::string_view&k) const
        {
            static Variant bad;
            auto i = m_tags.find(k);
            if(i != m_tags.end())
                return i->second;
            return bad;
        }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        Meta::Writer&     Meta::Writer::alias(const char zAlias[])
        {
            assert(zAlias);
            assert(strnlen(zAlias, MAX_NAME+1) <= MAX_NAME);
            m_meta -> m_aliases << std::string_view(zAlias);
            return *this;
        }
        
        Meta::Writer&     Meta::Writer::description(const char zDescription[])
        {
            assert(zDescription);
            assert(strnlen(zDescription, MAX_DESCRIPTION+1) <= MAX_DESCRIPTION);
            m_meta -> m_description = std::string_view(zDescription);
            return *this;
        }
        
        Meta::Writer&     Meta::Writer::label(const char zLabel[])
        {
            assert(zLabel);
            assert(strnlen(zLabel, MAX_LABEL+1) <= MAX_LABEL);

            m_meta -> m_label = std::string_view(zLabel);
            return *this;
        }
        
        Meta::Writer&     Meta::Writer::tag(const char zKey[])
        {
            assert(zKey);
            assert(strnlen(zKey, MAX_KEY+1) <= MAX_KEY);
            assert("Tag already set!" && !m_meta->m_tags.has(zKey));
            m_meta -> m_tags[zKey] = Variant(true);
            return *this;
        }
        
        Meta::Writer&     Meta::Writer::tag(const char zKey[], Variant&& value)
        {
            assert(zKey);
            assert(strnlen(zKey, MAX_KEY+1) <= MAX_KEY);
            assert("Tag already set!" && !m_meta->m_tags.has(zKey));
            m_meta -> m_tags[zKey] = std::move(value);
            return *this;
        }
        

        Meta::Writer&     Meta::Writer::tag(const char zKey[], const Variant& value)
        {
            assert(zKey);
            assert(strnlen(zKey, MAX_KEY+1) <= MAX_KEY);
            assert("Tag already set!" && !m_meta->m_tags.has(zKey));
            m_meta -> m_tags[zKey] = value;
            return *this;
        }
        
        Meta::Writer&   Meta::Writer::tls()
        {
            m_meta -> m_flags |= TLS;
            return *this;
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  META OBJECT
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  METHOD
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        MethodInfo::MethodInfo(const char* zName, Meta* parentMeta, options_t opts) : Meta(zName, parentMeta)
        {
            assert(parentMeta);

            m_flags |= METHOD | opts;

            if(GlobalInfo* g = to_global(parentMeta)){
                assert(g == &meta<Global>());
                if(g->m_methods.keys.has(zName))
                    metaCritical << "Duplicate method on GLOBAL: " << zName;
                g->m_methods << this;
            }
            
            if(ObjectInfo* obj = to_object(parentMeta)){
                if(obj->m_local.methods.keys.has(zName))
                    metaCritical << "Duplicate method on object (" << obj -> name() << "): " << zName;
                obj->m_local.methods << this;
            }
            
            if(TypeInfo* type = to_type(parentMeta)){
                if(type -> m_methods.keys.has(zName))
                    metaCritical << "Duplicate method on type (" << type -> name() << "): " << zName;
                type->m_methods << this;
            }
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  OBJECT (INFO)
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectInfo::ObjectInfo(const char* zName, const char* zFile, ObjectInfo* myBase) : CompoundInfo(zName, zFile), m_base(myBase)
        {
            assert(zFile);
            
            m_flags |= OBJECT;

            Repo& r    = repo();
            if(r.objects.lut.has(zName))
                metaCritical << "Duplicate object registration: " << zName;
            r.objects << this;
        }


        void    ObjectInfo::sweep_impl() 
        {
            CompoundInfo::sweep_impl();
            
            m_all           = {};
            m_local.bases   = {};
            m_local.derived = {};

            if(m_base){
                m_local.bases << m_base;
                m_base -> sweep();
                m_base -> m_local.derived << this;
                
                m_all.bases += m_base -> m_all.bases;
                m_all.methods += m_base -> m_all.methods;
                m_all.properties += m_base -> m_all.properties;
                for(ObjectInfo* b = m_base; b; b = b -> m_base)
                    m_all.derived << this;
            }
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  PROPERTY
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        PropertyInfo::PropertyInfo(const char* zName, const TypeInfo&theType, Meta* parentMeta, options_t opts) : Meta(zName, parentMeta), m_type(theType)
        {
            assert(parentMeta);

            m_flags |= PROPERTY | opts;

            if(GlobalInfo* g = to_global(parentMeta)){
                assert(g == &meta<Global>());
                if(g->m_properties.keys.has(zName))
                    metaCritical << "Duplicate property on GLOBAL: " << zName;
                g->m_properties << this;
            }
            
            if(ObjectInfo* obj = to_object(parentMeta)){
                if(obj->m_local.properties.keys.has(zName))
                    metaCritical << "Duplicate property on object (" << obj->name() << "): " << zName;
                obj->m_local.properties << this;
            }
            
            if(TypeInfo* type = to_type(parentMeta)){
                if(type -> m_properties.keys.has(zName))
                    metaCritical << "Duplicate property on type (" << type->name() << "): " << zName;
                type->m_properties << this;
            }
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  PROP GETTER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        PropGetter::PropGetter(PropertyInfo* propInfo) : Meta("get", propInfo) 
        {
            assert("no duplicate getters!" && !propInfo->m_getter);    //  duplicate property is an ERROR
            propInfo->m_getter     = this;
        }

        const PropertyInfo* PropGetter::property() const
        {
            return static_cast<const PropertyInfo*>(parent());
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  PROP SETTER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        PropSetter::PropSetter(PropertyInfo* propInfo) : Meta("", propInfo) 
        {
            assert("no duplicate setters!" && !propInfo->m_setter);    //  duplicate property is an ERROR
            propInfo->m_setter     = this;
        }

        const PropertyInfo* PropSetter::property() const
        {
            return static_cast<const PropertyInfo*>(parent());
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  TYPE
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        const Vector<const TypeInfo*>&   TypeInfo::all()
        {
            assert(thread_safe_read());
            return repo().types.all;
        }
        
        const TypeInfo*                  TypeInfo::lookup(id_t i)
        {
            assert(thread_safe_read());
            const Meta* m   = repo().all.value(i, nullptr);
            return (m && m->is_type()) ? static_cast<const TypeInfo*>(m) : nullptr;
        }
        
        const TypeInfo*                  TypeInfo::lookup(const std::string_view&sv)
        {
            assert(thread_safe_read());
            return repo().types.lut.first(sv, nullptr);
        }


        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        TypeInfo::TypeInfo(const char* zName, const char* zFile, id_t i) : CompoundInfo(zName, zFile, nullptr, i)
        {
            m_flags |= TYPE;
            
            Repo&   r  = repo();
            assert("no duplicate typenames!" && !r.types.lut.has(zName)); 
            
            r.types << this;
        }

        TypeInfo::~TypeInfo()
        {
            //  should never be used....
        }
        
        void    TypeInfo::sweep_impl() 
        {
            CompoundInfo::sweep_impl();
            gather(m_methods);
            gather(m_properties);
        }
}




YQ_INVOKE(
    yq::invalid();
    yq::variant();
    yq::GlobalInfo::instance();
);




#if 0
    //  SIMPLY AN EXAMPLE TO USAGE
void        yq_invoke()
{
    meta::object<T>().description().arg()
        << meta::description("")
        << meta::property("name", mv/getter/setter )
        << meta::tag()
    ;
    meta::function<T::FN>() << meta::Arg()
    meta::value<T>()
    meta::method<T::FN>().description().arg()....
    meta::
}
#endif
