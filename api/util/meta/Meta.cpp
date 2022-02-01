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


    bool    meta_unlocked()
    {
        return repo().openReg;
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
            return repo().all;
        }

        void    Meta::sweep_all()
        {
            Repo&   r   = repo();
            if(!r.openReg)
                return ;
        }


        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        Meta::Meta(const char zName[], id_t i) : Meta(zName, nullptr, i)
        {
        }
        
        Meta::Meta(const char zName[], Meta* parent, id_t i) 
        {
            //  strip out the yq namespace
            m_name      = str_start(zName, "yq::");
            m_parent    = parent;

            auto& _r     = repo();
            if(_r.openReg){
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
        }
        
        Meta::~Meta()
        {
            assert(false && "Do NOT delete Meta!  It's not meant to be deleted.");
        }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        Meta::Writer&     Meta::Writer::alias(const char z[])
        {
            if(z && m_meta && meta_unlocked())
                m_meta -> m_aliases << std::string_view(z);
            return *this;
        }
        
        Meta::Writer&     Meta::Writer::description(const char z[])
        {
            if(z && m_meta && meta_unlocked())
                m_meta -> m_description = std::string_view(z);
            return *this;
        }
        
        Meta::Writer&     Meta::Writer::label(const char z[])
        {
            if(z && m_meta && meta_unlocked())
                m_meta -> m_label = std::string_view(z);
            return *this;
        }
        
        Meta::Writer&     Meta::Writer::tag(const char z[], const Variant& v)
        {
            if(z && m_meta && meta_unlocked())
                m_meta -> m_tags[z] = v;
            return *this;
        }
        
        Meta::Writer&   Meta::Writer::tls()
        {
            if(m_meta && meta_unlocked())
                m_meta -> m_flags |= TLS;
            return *this;
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  METHOD
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        MethodInfo::MethodInfo(const char* zName, Meta*par, options_t opts) : Meta(zName, par)
        {
            m_flags |= METHOD | opts;
            if(zName && par && meta_unlocked()){
                if(par -> m_flags & GLOBAL){
                    GlobalInfo* g   = static_cast<GlobalInfo*>(par);
                    if(g->m_methods.keys.has(zName))
                        metaCritical << "Duplicate method on GLOBAL: " << zName;
                    g->m_methods << this;
                }
                
                if(par -> m_flags & OBJECT){
                    ObjectInfo* obj = static_cast<ObjectInfo*>(par);
                    if(obj->m_local.methods.keys.has(zName)){
                        std::string_view   pp  = par->parent() ?  par->parent()->name() : "(unnamed)";
                        metaCritical << "Duplicate method on object (" << pp << "): " << zName;
                    }
                    obj->m_local.methods << this;
                }
                
                if(par -> m_flags & TYPE){
                    TypeInfo*   type = static_cast<TypeInfo*>(par);
                    if(type -> m_methods.keys.has(zName)){
                        std::string_view   pp  = par->parent() ?  par->parent()->name() : "(unnamed)";
                        metaCritical << "Duplicate method on type (" << pp << "): " << zName;
                    }
                    type->m_methods << this;
                }
            }
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  OBJECT (INFO)
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectInfo::ObjectInfo(const char* zName, const char* zFile, ObjectInfo*b) : CompoundInfo(zName, zFile), m_base(b)
        {
            m_flags |= OBJECT;
            Repo& r    = repo();
            if(r.openReg){
                if(r.objects.lut.has(zName))
                    metaCritical << "Duplicate object registration: " << zName;
                r.objects << this;
            }
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
    //  OBJECT (META)
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  PROPERTY
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        PropertyInfo::PropertyInfo(const char* zName, const TypeInfo&t, Meta*par, options_t opts) : Meta(zName, par), m_type(t)
        {
            m_flags |= PROPERTY | opts;
            if(zName && par && meta_unlocked()){
                if(par -> m_flags & GLOBAL){
                    GlobalInfo* g   = static_cast<GlobalInfo*>(par);
                    if(g->m_properties.keys.has(zName))
                        metaCritical << "Duplicate property on GLOBAL: " << zName;
                    g->m_properties << this;
                }
                
                if(par -> m_flags & OBJECT){
                    ObjectInfo* obj = static_cast<ObjectInfo*>(par);
                    if(obj->m_local.properties.keys.has(zName)){
                        std::string_view   pp  = par->parent() ?  par->parent()->name() : "(unnamed)";
                        metaCritical << "Duplicate property on object (" << pp << "): " << zName;
                    }
                    obj->m_local.properties << this;
                }
                
                if(par -> m_flags & TYPE){
                    TypeInfo*   type = static_cast<TypeInfo*>(par);
                    if(type -> m_properties.keys.has(zName)){
                        std::string_view   pp  = par->parent() ?  par->parent()->name() : "(unnamed)";
                        metaCritical << "Duplicate property on type (" << pp << "): " << zName;
                    }
                    type->m_properties << this;
                }
            }
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  PROP GETTER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        PropGetter::PropGetter(PropertyInfo*m) : Meta("", m) 
        {
            if(m && meta_unlocked()){
                if(m->m_getter){
                    std::string_view   pp  = m->parent() ?  m->parent()->name() : "(unnamed)";
                    metaCritical  << "Duplicate getter attempted on property " << pp << "::" << m->name();
                }
                m->m_getter     = this;
            }
        }

        const PropertyInfo* PropGetter::property() const
        {
            return static_cast<const PropertyInfo*>(parent());
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  PROP SETTER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        PropSetter::PropSetter(PropertyInfo*m) : Meta("", m) 
        {
            if(m && meta_unlocked()){
                if(m->m_setter){
                    std::string_view   pp  = m->parent() ?  m->parent()->name() : "(unnamed)";
                    metaCritical  << "Duplicate setter attempted on property " << pp << "::" << m->name();
                }
                m->m_setter     = this;
            }
        }

        const PropertyInfo* PropSetter::property() const
        {
            return static_cast<const PropertyInfo*>(parent());
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  TYPE
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        TypeInfo::TypeInfo(const char* zName, const char* zFile, id_t i) : CompoundInfo(zName, zFile, nullptr, i)
        {
            m_flags |= TYPE;
            Repo&   r  = repo();
            if(r.openReg){
                if(r.types.keys.has(zName))
                    metaCritical  << "Duplicate type regsitration: " << zName;
                r.types << this;
            }
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
    

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  VARIANT
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Variant::Variant(const Variant& cp) : m_type(cp.m_type)
        {
            assert(m_type);
            assert(m_type -> m_ctorCopyB);

            (m_type -> m_ctorCopyB)(m_data, cp.m_data);
        }
        
        Variant::Variant(Variant&&mv) : m_type(mv.m_type)
        {
            (m_type -> m_ctorMove)(m_data, std::move(mv.m_data));
            mv.m_type   = &invalid();
        }
        
        #if 0
        Variant(char);
        Variant(char8_t);
        Variant(char32_t);
        Variant(const char*);
        Variant(const char8_t*);
        Variant(const char32_t*);
        Variant(const std::string&);
        Variant(const std::u8string&);
        Variant(const std::u32string&);
        Variant(const std::wstring&);
        Variant(const String&);
    #ifdef ENABLE_QT
        Variant(const QString&);
    #endif
        #endif
        
        
        //! Creates a defaulted Variant to the specified meta-type
        Variant::Variant(const TypeInfo&mt) : m_type(&mt)
        {
            assert(m_type);
            assert(m_type -> m_ctorCopyB);

            (m_type -> m_ctorCopyB)(m_data, m_type -> m_default);
        }
        
        //  Variant(const meta::Type*);

        Variant::~Variant()
        {
            assert(m_type);
            (m_type -> m_dtor)(m_data);
            
            #ifndef NDEBUG
            m_type      = &invalid();
            #endif
        }
        

        Variant&        Variant::operator=(const Variant& cp)
        {
            assert(m_type);

            if(&cp != this){
                if(cp.m_type == m_type){
                    assert(m_type -> m_copyB);

                    (m_type -> m_copyB)(m_data, cp.m_data);
                } else {
                    assert(m_type -> m_dtor);
                    assert(m_type -> m_ctorCopyB);

                    (m_type -> m_dtor)(m_data);
                    m_type  = cp.m_type;
                    (m_type -> m_ctorCopyB)(m_data, cp.m_data);
                }
            }
            return *this;
        }
        
        Variant&        Variant::operator=(Variant&&mv)
        {
            assert(m_type);
            if(&mv != this){
                if(mv.m_type == m_type){
                    assert(m_type -> m_moveB);

                    (m_type -> m_moveB)(m_data, std::move(mv.m_data));
                } else {
                    assert(m_type -> m_dtor);
                    assert(m_type -> m_ctorMove);
                    
                    (m_type -> m_dtor)(m_data);
                    m_type  = mv.m_type;
                    (m_type -> m_ctorMove)(m_data, std::move(mv.m_data));
                }
                mv.m_type   = &invalid();
            }
            return *this;
        }
        
        bool            Variant::operator==(const Variant&b) const
        {
            assert(m_type);
            assert(m_type -> m_equal);
        
            if(this == &b)
                return true;
            if(m_type != b.m_type)
                return false;
            return (m_type->m_equal)(m_data, b.m_data);
        }
        

        bool            Variant::can_convert_to(const TypeInfo&t) const
        {
            assert(m_type);
            if(m_type == &t)
                return true;
            return m_type->m_convert.has(&t);
        }

        const void*      Variant::raw_ptr() const 
        {
            return m_type -> is_small() ? (const void*) m_data.Data : m_data.Pointer;
        };
        
        void*            Variant::raw_ptr() 
        {
            return m_type -> is_small() ? (void*) m_data.Data : m_data.Pointer;
        };

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
