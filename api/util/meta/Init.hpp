////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Binder.hpp"
#include "Global.hpp"
#include "ObjectInfo.hpp"
#include "TypeInfo.hpp"

#include <util/c++/TypeTraits.hpp>
#include <util/collection/List.hpp>
#include <util/collection/Hash.hpp>
#include <util/collection/Map.hpp>
#include <util/collection/MultiMap.hpp>
#include <util/collection/Set.hpp>
#include <util/collection/Vector.hpp>
#include <util/log/Logging.hpp>


#define metaAlert       yAlert("meta")
#define metaCritical    yCritical("meta")
#define metaDebug       yDebug("meta")
#define metaError       yError("meta")
#define metaEmergency   yEmergency("meta")
#define metaFatal       yFatal("meta")
#define metaInfo        yInfo("meta")
#define metaNotice      yNotice("meta")
#define metaWarning     yWarning("meta")


#define MT_IMPLEMENT( ... )                                                                         \
    namespace yq {                                                                                  \
        TypeInfo&   InfoBinder<__VA_ARGS__>::edit() {                                               \
            static auto* s_ret   = new TypeInfo::Final<__VA_ARGS__>(#__VA_ARGS__, __FILE__);        \
            return *s_ret;                                                                          \
        }                                                                                           \
    } }

#define MT_FIXED( i, ... )                                                                          \
    namespace yq {                                                                                  \
        TypeInfo&   InfoBinder<__VA_ARGS__>::edit() {                                               \
            static auto* s_ret   = new TypeInfo::Final<__VA_ARGS__>(#__VA_ARGS__, __FILE__, i);     \
            return *s_ret;                                                                          \
        }                                                                                           \
    }
    
#define MO_IMPLEMENT( ... )                                                                         \
    namespace yq {                                                                                  \
    }

namespace yq {

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  META WRITER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////

        class Meta::Writer {
        public:
        
            Writer&     alias(const char z[]);
            Writer&     description(const char []);
            Writer&     label(const char []);
            Writer&     tag(const char [], const Variant& v = Variant());
            Writer&     tls();  //!< Marks the TLS flag ... only meaningful on static variables, still
        
        protected:
                //  In proper usage, should never be null, however, guard!
            Meta* const m_meta = nullptr;
            
            Writer(Meta* m) : m_meta(m) {}
        };

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  GETTERS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////

        /*! \brief Abstract PropGetter
            
            This is an abstract "getter" that may or may not have an object associated with it.
        */
        class PropGetter : public Meta {
        public:
            virtual bool            get(void*, const void*) const = 0;
            virtual const Meta&     object() const = 0;
            virtual const Meta&     data() const = 0;
            
            const PropertyInfo* property() const;

        protected:
            PropGetter(PropertyInfo*);
        };
        
        
        template <typename T>
        class StaticPropGetter : public PropGetter {
        public:
        
            virtual const Meta&     data() const override
            {
                return meta<T>();
            }

            virtual const Meta&     object() const override
            {
                return invalid();
            }

        protected:
            StaticPropGetter(PropertyInfo*p) : PropGetter(p) {}
        };
        
        /*! \brief PropGetter for a simple pointer variable
        
            Simple getter for the scenario of a global scoped varaible.  
            No object pointer required.
        */
        template <typename T>
        class XPV_PropGetter : public StaticPropGetter<T> {
        public:
            typedef T* P;
            XPV_PropGetter(PropertyInfo*m, P ptr) : StaticPropGetter<T>(m), m_data(ptr) 
            {
                assert(ptr);
            }
        
            virtual bool            get(void*dst, const void*) const override
            {
                if(dst){
                    *(T*) dst   = *m_data;
                    return true;
                } else
                    return false;
            }
            
            
        private:
            P      m_data;
        };
        
        template <typename T>
        class XFR_PropGetter : public StaticPropGetter<T> {
        public:
            typedef T (*FN)();
            XFR_PropGetter(PropertyInfo* m, FN fn) : StaticPropGetter<T>(m), m_fn(fn) 
            {
                assert(fn);
            }
            
            virtual bool    get(void* dst, const void*) const  override
            {
                if(dst){
                    *(T*) dst   = std::move(m_fn());
                    return true;
                } else
                    return false;
            }

        private:
            FN      m_fn;
        };
        
        template <typename T>
        class XFVR_PropGetter : public StaticPropGetter<T> {
        public:
            typedef void (*FN)(const T&);
            XFVR_PropGetter(PropertyInfo* m, FN fn) : StaticPropGetter<T>(m), m_fn(fn)
            {
                assert(fn);
            }
            
            virtual bool    get(void* dst, const void*) const override
            {
                if(dst){
                    m_fn(*(T*) dst);
                    return true;
                } else
                    return false;
            }

        private:
            FN      m_fn;
        };
        
        template <typename T>
        class XFBR_PropGetter : public StaticPropGetter<T> {
        public:
            typedef bool (*FN)(const T&);
            XFBR_PropGetter(PropertyInfo* m, FN fn) : StaticPropGetter<T>(m), m_fn(fn)
            {
                assert(fn);
            }
            
            virtual bool    get(void* dst, const void*) const override
            {
                if(dst){
                    return m_fn(*(T*) dst);
                } else
                    return false;
            }
        private:
            FN      m_fn;
        };
        
            //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        
        template <typename C, typename T>
        class DynamicPropGetter : public PropGetter {
        public:

            virtual const Meta&     data() const override
            {
                return meta<T>();
            }

            virtual const Meta&     object() const override
            {
                return meta<C>();
            }

        protected:
            DynamicPropGetter(PropertyInfo* m) : PropGetter(m) {};
        };
        
    
        /* !\brief PropGetter for a member variable
        
            PropGetter for the scenario of a member variable on an object.
        */
        template <typename C, typename T>
        class IPM_PropGetter : public DynamicPropGetter<C,T> {
        public:
            typedef T (C::*P);
            IPM_PropGetter(PropertyInfo*m, P ptr) : DynamicPropGetter<C,T>(m), m_data(ptr) 
            {
            }

            virtual bool            get(void*dst, const void*obj) const override
            {
                if(dst&&obj){
                    *(T*) dst   = *(((C*) obj)->*m_data);
                    return true;
                } else
                    return false;
            }
            
        private:
            P       m_data;
        };
        
        template <typename C, typename T>
        class IFV_PropGetter : public DynamicPropGetter<C,T> {
        public:
            typedef T (C::*FN)() const;
            IFV_PropGetter(PropertyInfo*m, FN fn) : DynamicPropGetter<C,T>(m), m_fn(fn)
            {
            }
            
            virtual bool        get(void* dst, const void* obj) const override
            {
                if(dst && obj){
                    *(T*) dst   = (((const C*) obj)->*m_fn)();
                    return true;
                } else
                    return false;
            }
            
        private:
            FN      m_fn;
        };
        
        
        template <typename C, typename T>
        class IFR_PropGetter : public DynamicPropGetter<C,T> {
        public:
            typedef const T& (C::*FN)() const;
            IFR_PropGetter(PropertyInfo*m, FN fn) : DynamicPropGetter<C,T>(m), m_fn(fn)
            {
            }
            
            virtual bool        get(void* dst, const void* obj) const override
            {
                if(dst && obj){
                    *(T*) dst   = (((const C*) obj)->*m_fn)();
                    return true;
                } else
                    return false;
            }
            
        private:
            FN      m_fn;
        };
        

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  SETTERS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////


        class PropSetter : public Meta {
        public:
            virtual bool            set(void*, const void*) const = 0;
            virtual const Meta&     data() const = 0;
            virtual const Meta&     object() const = 0;
            const PropertyInfo* property() const;
        protected:
            PropSetter(PropertyInfo*);
        };
        
        template <typename T>
        class StaticPropSetter : public PropSetter {
        public:
    
            virtual const Meta&     data() const override
            {
                return meta<T>();
            }
            
            virtual const Meta&     object() const override
            {
                return invalid();
            }

        protected:
            StaticPropSetter(PropertyInfo*p) : PropSetter(p) {}
        };
        
        template <typename T>
        class XPV_PropSetter : public StaticPropSetter<T> {
        public:
            typedef T* P;
            XPV_PropSetter(PropertyInfo*m, P ptr) : StaticPropSetter<T>(m), m_data(ptr) 
            {
                assert(p);
            }
            
            virtual bool            set(void*, const void*src) const override
            {
                if(src){
                    *m_data = *(const T*) src;
                    return true;
                }
                return false;
            }
            

        private:
            P      m_data;
        };

        template <typename T>
        class XFV_PropSetter : public StaticPropSetter<T> {
        public:
            typedef void (*FN)(T);
            XFV_PropSetter(PropertyInfo*m, FN fn) : StaticPropSetter<T>(m), m_fn(fn) 
            {
                assert(p);
            }
            
            virtual bool            set(void*, const void*src) const override
            {
                if(src){
                    (*m_fn)(*(const T*) src);
                    return true;
                }
                return false;
            }
            

        private:
            FN      m_fn;
        };

        template <typename T>
        class XFBV_PropSetter : public StaticPropSetter<T> {
        public:
            typedef bool (*FN)(T);
            XFBV_PropSetter(PropertyInfo*m, FN fn) : StaticPropSetter<T>(m), m_fn(fn) 
            {
                assert(p);
            }
            
            virtual bool            set(void*, const void*src) const override
            {
                if(src)
                    return (*m_fn)(*(const T*) src);
                return false;
            }
            

        private:
            FN      m_fn;
        };

        template <typename T>
        class XFR_PropSetter : public StaticPropSetter<T> {
        public:
            typedef void (*FN)(const T&);
            XFR_PropSetter(PropertyInfo*m, FN fn) : StaticPropSetter<T>(m), m_fn(fn) 
            {
                assert(p);
            }
            
            virtual bool            set(void*, const void*src) const override
            {
                if(src){
                    (*m_fn)(*(const T*) src);
                    return true;
                }
                return false;
            }
            

        private:
            FN      m_fn;
        };


        template <typename T>
        class XFBR_PropSetter : public StaticPropSetter<T> {
        public:
            typedef bool (*FN)(const T&);
            XFBR_PropSetter(PropertyInfo*m, FN fn) : StaticPropSetter<T>(m), m_fn(fn) 
            {
                assert(p);
            }
            
            virtual bool            set(void*, const void*src) const override
            {
                if(src)
                    return (*m_fn)(*(const T*) src);
                return false;
            }
            

        private:
            FN      m_fn;
        };


                //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        
        template <typename C, typename T>
        class DynamicPropSetter : public PropSetter {
        public:
        
            virtual const Meta&     data() const override
            {
                return meta<T>();
            }

            virtual const Meta&     object() const override
            {
                return meta<C>();
            }

        protected:
            DynamicPropSetter(PropertyInfo* m) : PropSetter(m) {}
        };

        
        template <typename C, typename T>
        class IPM_PropSetter : public DynamicPropSetter<C,T> {
        public:
            typedef T*(C::*P);
            IPM_PropSetter(PropertyInfo*m, P ptr) : DynamicPropSetter<C,T>(m), m_data(ptr) {}
            
            virtual bool            set(void*dst, const void*src) const override
            {
                if(src&&dst){
                    *(((C*) dst)->*m_data) =  *(const T*) src;
                    return true;
                }
                return false;
            }
        
            
        private:
            P      m_data;
        };
        
        template <typename C, typename T>
        class IFV_PropSetter : public DynamicPropSetter<C,T>{
        public:
            typedef void (C::*FN)(T);
            
            IFV_PropSetter(PropertyInfo*m, FN fn) : DynamicPropSetter<C,T>(m), m_fn(fn) {}
            
            virtual bool            set(void* dst, const void* src) const override
            {
                if(src && dst){
                    (((C*) dst)->*m_fn)(*(const T*) src);
                    return true;
                }
                return false;
            }
            
        private:
            FN      m_fn;
        };

        template <typename C, typename T>
        class IFBV_PropSetter : public DynamicPropSetter<C,T>{
        public:
            typedef bool (C::*FN)(T);
            
            IFBV_PropSetter(PropertyInfo*m, FN fn) : DynamicPropSetter<C,T>(m), m_fn(fn) {}
            
            virtual bool            set(void* dst, const void* src) const override
            {
                if(src && dst)
                    return (((C*) dst)->*m_fn)(*(const T*) src);
                return false;
            }
            
        private:
            FN      m_fn;
        };

        template <typename C, typename T>
        class IFR_PropSetter : public DynamicPropSetter<C,T>{
        public:
            typedef void (C::*FN)(const T&);
            
            IFR_PropSetter(PropertyInfo*m, FN fn) : DynamicPropSetter<C,T>(m), m_fn(fn) {}
            
            virtual bool            set(void* dst, const void* src) const override
            {
                if(src && dst){
                    (((C*) dst)->*m_fn)(*(const T*) src);
                    return true;
                }
                return false;
            }
            
        private:
            FN      m_fn;
        };

        template <typename C, typename T>
        class IFBR_PropSetter : public DynamicPropSetter<C,T>{
        public:
            typedef bool (C::*FN)(const T&);
            
            IFBR_PropSetter(PropertyInfo*m, FN fn) : DynamicPropSetter<C,T>(m), m_fn(fn) {}
            
            virtual bool            set(void* dst, const void* src) const override
            {
                if(src && dst)
                    return (((C*) dst)->*m_fn)(*(const T*) src);
                return false;
            }
            
        private:
            FN      m_fn;
        };

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  PROPERTIES
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////


        template <typename T>
        class PropertyInfo::Writer : public Meta::Writer {
        public:
            Writer&  defValueInfo(const T& val)
            {
                if(m_meta && meta_unlocked())
                    static_cast<PropertyInfo*>(m_meta) -> m_default.reference<T>() = val;
                return *this;
            }
        };
        
     //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


        template <typename T>
        class PropertyInfo::VarW : public Writer<T> {
        public:
        
            Writer<T>&     setter(void(*p)(T))
            {
                if(p && Meta::Writer::m_meta && meta_unlocked())
                    new XFV_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                return *this;
            }
            
            Writer<T>&     setter(void(*p)(const T&))
            {
                if(p && Meta::Writer::m_meta && meta_unlocked())
                    new XFR_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                return *this;
            }
            
            Writer<T>&     setter(bool(*p)(T))
            {
                if(p && Meta::Writer::m_meta && meta_unlocked())
                    new XFBV_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                return *this;
            }
            
            Writer<T>&     setter(bool(*p)(const T&))
            {
                if(p && Meta::Writer::m_meta && meta_unlocked())
                    new XFBR_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                return *this;
            }
        };
    
        template <typename C, typename T>
        class PropertyInfo::PropW : public Writer<T> {
        public:
        
            Writer<T>&     setter(void (C::*p)(T))
            {
                if(p && Meta::Writer::m_meta && meta_unlocked())
                    new IFV_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                return *this;
            }
            
            Writer<T>&     setter(void (C::*p)(const T&))
            {
                if(p && Meta::Writer::m_meta && meta_unlocked())
                    new IFR_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                return *this;
            }
            
            Writer<T>&     setter(bool (C::*p)(T))
            {
                if(p && Meta::Writer::m_meta && meta_unlocked())
                    new IFBV_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                return *this;
            }
            
            Writer<T>&     setter(bool (C::*p)(const T&))
            {
                if(p && Meta::Writer::m_meta && meta_unlocked())
                    new IFBR_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                return *this;
            }
        
        };
    

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  METHODS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        class MethodInfo::Writer : public Meta::Writer {
        public:
        
        };


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  COMPOUND
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////


        class CompoundInfo::Static : public Meta::Writer {
        public:
            
            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read/write.
                
                \tparam T   type
            */
            template <typename T>
            PropertyInfo::Writer<T>       variable(const char szName[], T*p)
            {
                if(p && szName && meta_unlocked()){
                    PropertyInfo*ret  = new PropertyInfo(szName, meta<T>(), m_meta, STATIC);
                    new XPV_PropGetter<T>(ret, p);
                    new XPV_PropSetter<T>(ret, p);
                    return PropertyInfo::Writer<T>{ret};
                } else
                    return {};
            }

            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read only.
                
                \tparam T   type
            */
            template <typename T>
            PropertyInfo::VarW<T>           variable(const char szName[], T (*fn)())
            {
                if(fn && szName && meta_unlocked()){
                    PropertyInfo*   ret = new PropertyInfo(szName, meta<T>(), m_meta, STATIC);
                    new XFR_PropGetter<T>(ret, fn);
                    return PropertyInfo::VarW<T>(ret);
                } else
                    return {};
            }

            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read only.
                
                \tparam T   type
            */
            template <typename T>
            PropertyInfo::VarW<T>           variable(const char szName[], void (*fn)(T&))
            {
                if(fn && szName && meta_unlocked()){
                    PropertyInfo*   ret = new PropertyInfo(szName, meta<T>(), m_meta, STATIC);
                    new XFVR_PropGetter<T>(ret, fn);
                    return PropertyInfo::VarW<T>(ret);
                } else
                    return {};
            }
            

            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read only.
                
                \tparam T   type
            */
            template <typename T>
            PropertyInfo::VarW<T>           variable(const char szName[], bool (*fn)(T&))
            {
                if(fn && szName && meta_unlocked()){
                    PropertyInfo*   ret = new PropertyInfo(szName, meta<T>(), m_meta, STATIC);
                    new XFBR_PropGetter<T>(ret, fn);
                    return PropertyInfo::VarW<T>(ret);
                } else
                    return {};
            }



            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read/write.
                
                \tparam T   type
            */
            template <typename ... Args>
            MethodInfo::Writer              function(const char szName[], void(*)(Args...));

            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read/write.
                
                \tparam T   type
            */
            template <typename T, typename ... Args>
            MethodInfo::Writer              function(const char szName[], T(*)(Args...));

        protected: 
            Static( CompoundInfo* c ) : Meta::Writer(c) {}

        };
        
        template <typename C>
        class CompoundInfo::Dynamic : public CompoundInfo::Static {
        public:
        
            /*! \brief Defines a property
            
                This defines a property for the type/object
            
                \tparam T       type
                \param  p       Pointer to class/type member
            */
            template <typename T>
            PropertyInfo::Writer<T>     property(const char szName[], T (C::*p))
            {
                if(p && szName && meta_unlocked()){
                    PropertyInfo*ret  = new PropertyInfo(szName, meta<T>(), m_meta, STATE);
                    new IPM_PropGetter<C,T>(ret, p);
                    new IPM_PropSetter<C,T>(ret, p);
                    return PropertyInfo::Writer<T>{ret};
                } else
                    return {};
            }
            

            /*! \brief Defines a property
            
                This defines a property for the type/object
            
                \tparam T       type
                \param  p       Function pointer to getter (const & returns)
            */
            template <typename T>
            PropertyInfo::PropW<C,T>    property(const char szName[], T (C::*p)() const)
            {
                if(p && szName && meta_unlocked()){
                    PropertyInfo*ret  = new PropertyInfo(szName, meta<T>(), m_meta);
                    new IFV_PropGetter<C,T>(ret, p);
                    return PropertyInfo::PropW<C,T>{ret};
                } else 
                    return {};
            }
            
            /*! \brief Defines a property
            
                This defines a property for the type/object
            
                \tparam T       type
                \param  p       Function pointer to getter (const & returns)
            */
            template <typename T>
            PropertyInfo::PropW<C,T>    property(const char szName[], const T& (C::*p)() const)
            {
                if(p && szName && meta_unlocked()){
                    PropertyInfo*ret  = new PropertyInfo(szName, meta<T>(), m_meta);
                    new IFR_PropGetter<C,T>(ret, p);
                    return PropertyInfo::PropW<C,T>{ret};
                } else 
                    return {};
            }


            template <typename T>
            PropertyInfo::PropW<C,T>    property(const char szName[], void (C::*p)(T&) const);
        
            template <typename T>
            PropertyInfo::PropW<C,T>    property(const char szName[], bool (C::*p)(T&) const);
            
            template <typename ... Args>
            MethodInfo::Writer          method(const char szName[], void (C::*)(Args...));
            
            template <typename ... Args>
            MethodInfo::Writer          method(const char szName[], void (C::*)(Args...) const);

            template <typename T, typename ... Args>
            MethodInfo::Writer          method(const char szName[], T (C::*)(Args...));
            
            template <typename T, typename ... Args>
            MethodInfo::Writer          method(const char szName[], T (C::*)(Args...) const);
            
        protected:
            Dynamic(CompoundInfo* c) : Static(c) {}
        };


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  GLOBAL
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    class GlobalInfo::Writer : public CompoundInfo::Static {
    public:
    
        Writer(GlobalInfo* p) : CompoundInfo::Static(p) {}
    };

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  OBJECT
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    template <typename C>
    class ObjectInfo::Writer : public CompoundInfo::Dynamic<C> {
    public:
    
        template <typename B>
        Writer&     base()
        {
            static_assert( std::is_base_of_v<B, C>, "T must derive from B!" );
        }
        
        Writer(ObjectInfo* obj) : CompoundInfo::Dynamic<C>(obj) {}
    };


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  TYPE
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    template <typename T>
    struct TypeInfoFor {
        using Type  = TypeInfo;
    };
    
    template <typename T>
    using type_info_t  = TypeInfoFor<T>::Type;


    template <typename T>
    class TypeInfo::Typed : public type_info_t<T> {
    protected:
        Typed(const char* zName, const char* zFile, id_t i=AUTO_ID) : type_info_t<T>(zName, zFile, i)
        {
            TypeInfo::m_default.ctorCopy(T{});
        }
    };
    
    template <typename T>
    class TypeInfo::Special : public Typed<T> {
    public:
    
    protected:
        Special(const char* zName, const char* zFile, id_t i=AUTO_ID) : Typed<T>(zName, zFile, i) {}
    };

    //! The FINAL storage class, the one that's generic
    template <typename T>
    class TypeInfo::Final : public Special<T> {
    private:
        friend class InfoBinder<T>;
        Final(const char* zName, const char* zFile, id_t i=AUTO_ID) : Special<T>(zName, zFile, i) {}
    };


    template <typename T>
    class TypeInfo::Writer : public CompoundInfo::Dynamic<T> {
    public:
        static_assert( InfoBinder<T>::IsType, "T must be meta-type declared!");
    
        template <typename U>
        requires std::is_nothrow_convertible_v<U,T>
        void casts()
        {
            static_assert( InfoBinder<U>::IsType, "U must be meta-type declared!");
            if(meta_unlocked()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_convert[ &InfoBinder<U>::bind()] = [](void* dst, const void* src){
                    *(U*) dst = U( *(const T*) src);
                };
            }
        }
        
        Writer(TypeInfo* ti) : CompoundInfo::Dynamic<T>(ti) {}
    };

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  POINTER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  COLLECTION
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  STRUCT
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  VALUE
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////

}
