////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Binder.hpp"
#include "CompoundInfo.hpp"

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


namespace yq {
    namespace meta {
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
    
                /*! \brief Abstract Getter
                    
                    This is an abstract "getter" that may or may not have an object associated with it.
                */
                class Getter : public Meta {
                public:
                    virtual bool            get(void*, const void*) const = 0;
                    virtual const Meta&     object() const = 0;
                    virtual const Meta&     data() const = 0;
                    
                    const PropertyInfo* property() const;

                protected:
                    Getter(PropertyInfo*);
                };
                
                
                template <typename T>
                class StaticGetter : public Getter {
                public:
                
                    virtual const Meta&     data() const override
                    {
                        return metaOf<T>();
                    }

                    virtual const Meta&     object() const override
                    {
                        return invalid();
                    }

                protected:
                    StaticGetter(PropertyInfo*p) : Getter(p) {}
                };
                
                /*! \brief Getter for a simple pointer variable
                
                    Simple getter for the scenario of a global scoped varaible.  
                    No object pointer required.
                */
                template <typename T>
                class XPV_Getter : public StaticGetter<T> {
                public:
                    typedef T* P;
                    XPV_Getter(PropertyInfo*m, P ptr) : StaticGetter<T>(m), m_data(ptr) 
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
                class XFR_Getter : public StaticGetter<T> {
                public:
                    typedef T (*FN)();
                    XFR_Getter(PropertyInfo* m, FN fn) : StaticGetter<T>(m), m_fn(fn) 
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
                class XFVR_Getter : public StaticGetter<T> {
                public:
                    typedef void (*FN)(const T&);
                    XFVR_Getter(PropertyInfo* m, FN fn) : StaticGetter<T>(m), m_fn(fn)
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
                class XFBR_Getter : public StaticGetter<T> {
                public:
                    typedef bool (*FN)(const T&);
                    XFBR_Getter(PropertyInfo* m, FN fn) : StaticGetter<T>(m), m_fn(fn)
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
                class DynamicGetter : public Getter {
                public:

                    virtual const Meta&     data() const override
                    {
                        return metaOf<T>();
                    }

                    virtual const Meta&     object() const override
                    {
                        return metaOf<C>();
                    }

                protected:
                    DynamicGetter(PropertyInfo* m) : Getter(m) {};
                };
                
            
                /* !\brief Getter for a member variable
                
                    Getter for the scenario of a member variable on an object.
                */
                template <typename C, typename T>
                class IPM_Getter : public DynamicGetter<C,T> {
                public:
                    typedef T (C::*P);
                    IPM_Getter(PropertyInfo*m, P ptr) : DynamicGetter<C,T>(m), m_data(ptr) 
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
                class IFV_Getter : public DynamicGetter<C,T> {
                public:
                    typedef T (C::*FN)() const;
                    IFV_Getter(PropertyInfo*m, FN fn) : DynamicGetter<C,T>(m), m_fn(fn)
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
                class IFR_Getter : public DynamicGetter<C,T> {
                public:
                    typedef const T& (C::*FN)() const;
                    IFR_Getter(PropertyInfo*m, FN fn) : DynamicGetter<C,T>(m), m_fn(fn)
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
    

            class Setter : public Meta {
            public:
                virtual bool            set(void*, const void*) const = 0;
                virtual const Meta&     data() const = 0;
                virtual const Meta&     object() const = 0;
                const PropertyInfo* property() const;
            protected:
                Setter(PropertyInfo*);
            };
            
            template <typename T>
            class StaticSetter : public Setter {
            public:
        
                virtual const Meta&     data() const override
                {
                    return metaOf<T>();
                }
                
                virtual const Meta&     object() const override
                {
                    return invalid();
                }

            protected:
                StaticSetter(PropertyInfo*p) : Setter(p) {}
            };
            
            template <typename T>
            class XPV_Setter : public StaticSetter<T> {
            public:
                typedef T* P;
                XPV_Setter(PropertyInfo*m, P ptr) : StaticSetter<T>(m), m_data(ptr) 
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
            class XFV_Setter : public StaticSetter<T> {
            public:
                typedef void (*FN)(T);
                XFV_Setter(PropertyInfo*m, FN fn) : StaticSetter<T>(m), m_fn(fn) 
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
            class XFBV_Setter : public StaticSetter<T> {
            public:
                typedef bool (*FN)(T);
                XFBV_Setter(PropertyInfo*m, FN fn) : StaticSetter<T>(m), m_fn(fn) 
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
            class XFR_Setter : public StaticSetter<T> {
            public:
                typedef void (*FN)(const T&);
                XFR_Setter(PropertyInfo*m, FN fn) : StaticSetter<T>(m), m_fn(fn) 
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
            class XFBR_Setter : public StaticSetter<T> {
            public:
                typedef bool (*FN)(const T&);
                XFBR_Setter(PropertyInfo*m, FN fn) : StaticSetter<T>(m), m_fn(fn) 
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
            class DynamicSetter : public Setter {
            public:
            
                virtual const Meta&     data() const override
                {
                    return metaOf<T>();
                }

                virtual const Meta&     object() const override
                {
                    return metaOf<C>();
                }

            protected:
                DynamicSetter(PropertyInfo* m) : Setter(m) {}
            };

            
            template <typename C, typename T>
            class IPM_Setter : public DynamicSetter<C,T> {
            public:
                typedef T*(C::*P);
                IPM_Setter(PropertyInfo*m, P ptr) : DynamicSetter<C,T>(m), m_data(ptr) {}
                
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
            class IFV_Setter : public DynamicSetter<C,T>{
            public:
                typedef void (C::*FN)(T);
                
                IFV_Setter(PropertyInfo*m, FN fn) : DynamicSetter<C,T>(m), m_fn(fn) {}
                
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
            class IFBV_Setter : public DynamicSetter<C,T>{
            public:
                typedef bool (C::*FN)(T);
                
                IFBV_Setter(PropertyInfo*m, FN fn) : DynamicSetter<C,T>(m), m_fn(fn) {}
                
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
            class IFR_Setter : public DynamicSetter<C,T>{
            public:
                typedef void (C::*FN)(const T&);
                
                IFR_Setter(PropertyInfo*m, FN fn) : DynamicSetter<C,T>(m), m_fn(fn) {}
                
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
            class IFBR_Setter : public DynamicSetter<C,T>{
            public:
                typedef bool (C::*FN)(const T&);
                
                IFBR_Setter(PropertyInfo*m, FN fn) : DynamicSetter<C,T>(m), m_fn(fn) {}
                
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
                    if(m_meta && unlocked())
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
                    if(p && Meta::Writer::m_meta && unlocked())
                        new XFV_Setter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                    return *this;
                }
                
                Writer<T>&     setter(void(*p)(const T&))
                {
                    if(p && Meta::Writer::m_meta && unlocked())
                        new XFR_Setter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                    return *this;
                }
                
                Writer<T>&     setter(bool(*p)(T))
                {
                    if(p && Meta::Writer::m_meta && unlocked())
                        new XFBV_Setter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                    return *this;
                }
                
                Writer<T>&     setter(bool(*p)(const T&))
                {
                    if(p && Meta::Writer::m_meta && unlocked())
                        new XFBR_Setter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                    return *this;
                }
            };
        
            template <typename C, typename T>
            class PropertyInfo::PropW : public Writer<T> {
            public:
            
                Writer<T>&     setter(void (C::*p)(T))
                {
                    if(p && Meta::Writer::m_meta && unlocked())
                        new IFV_Setter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                    return *this;
                }
                
                Writer<T>&     setter(void (C::*p)(const T&))
                {
                    if(p && Meta::Writer::m_meta && unlocked())
                        new IFR_Setter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                    return *this;
                }
                
                Writer<T>&     setter(bool (C::*p)(T))
                {
                    if(p && Meta::Writer::m_meta && unlocked())
                        new IFBV_Setter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
                    return *this;
                }
                
                Writer<T>&     setter(bool (C::*p)(const T&))
                {
                    if(p && Meta::Writer::m_meta && unlocked())
                        new IFBR_Setter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), p);
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
                    if(p && szName && unlocked()){
                        PropertyInfo*ret  = new PropertyInfo(szName, metaOf<T>(), m_meta, STATIC);
                        new XPV_Getter<T>(ret, p);
                        new XPV_Setter<T>(ret, p);
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
                    if(fn && szName && unlocked()){
                        PropertyInfo*   ret = new PropertyInfo(szName, metaOf<T>(), m_meta, STATIC);
                        new XFR_Getter<T>(ret, fn);
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
                    if(fn && szName && unlocked()){
                        PropertyInfo*   ret = new PropertyInfo(szName, metaOf<T>(), m_meta, STATIC);
                        new XFVR_Getter<T>(ret, fn);
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
                    if(fn && szName && unlocked()){
                        PropertyInfo*   ret = new PropertyInfo(szName, metaOf<T>(), m_meta, STATIC);
                        new XFBR_Getter<T>(ret, fn);
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
                    if(p && szName && unlocked()){
                        PropertyInfo*ret  = new PropertyInfo(szName, metaOf<T>(), m_meta, STATE);
                        new IPM_Getter<C,T>(ret, p);
                        new IPM_Setter<C,T>(ret, p);
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
                    if(p && szName && unlocked()){
                        PropertyInfo*ret  = new PropertyInfo(szName, metaOf<T>(), m_meta);
                        new IFV_Getter<C,T>(ret, p);
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
                    if(p && szName && unlocked()){
                        PropertyInfo*ret  = new PropertyInfo(szName, metaOf<T>(), m_meta);
                        new IFR_Getter<C,T>(ret, p);
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
        class TypeInfo::Typed : public TypeInfo {
        public:
        
            Typed(const char* zName, const char* zFile, id_t i=AUTO_ID) : TypeInfo(zName, zFile, i)
            {
                m_defCopy.ctorCopy(T{});
                
            }
            
        };

        template <typename T>
        class TypeInfo::Writer : public CompoundInfo::Dynamic<T> {
        public:
            static_assert( Binder<T>::IsType, "T must be meta-type declared!");
        
            template <typename U>
            requires std::is_nothrow_convertible_v<U,T>
            void casts()
            {
                static_assert( Binder<U>::IsType, "U must be meta-type declared!");
                if(unlocked()){
                    static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_convert[ &Binder<U>::bind()] = [](void* dst, const void* src){
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
}
