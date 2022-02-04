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
#include <util/stream/Ops.hpp>


#define metaAlert       yAlert("meta")
#define metaCritical    yCritical("meta")
#define metaDebug       yDebug("meta")
#define metaError       yError("meta")
#define metaEmergency   yEmergency("meta")
#define metaFatal       yFatal("meta")
#define metaInfo        yInfo("meta")
#define metaNotice      yNotice("meta")
#define metaWarning     yWarning("meta")


#define MT_IMPLEMENT( ... )                                                                                 \
    namespace yq {                                                                                          \
        TypeInfo&   InfoBinder<__VA_ARGS__>::edit() {                                                       \
            static auto* s_ret   = new TypeInfo::Final<__VA_ARGS__>(#__VA_ARGS__, __FILE__);                \
            return *s_ret;                                                                                  \
        }                                                                                                   \
        template <> TypeInfo& TypeInfo::Final<__VA_ARGS__>::s_save  = InfoBinder<__VA_ARGS__>::edit();      \
    }

#define MT_FIXED( i, ... )                                                                                  \
    namespace yq {                                                                                          \
        TypeInfo&   InfoBinder<__VA_ARGS__>::edit() {                                                       \
            static auto* s_ret   = new TypeInfo::Final<__VA_ARGS__>(#__VA_ARGS__, __FILE__, i);             \
            return *s_ret;                                                                                  \
        }                                                                                                   \
        template <> TypeInfo& TypeInfo::Final<__VA_ARGS__>::s_save  = InfoBinder<__VA_ARGS__>::edit();      \
    }
    
#define MO_IMPLEMENT( ... )                                                                                 \
    namespace yq {                                                                                          \
    }

namespace yq {

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  META WRITER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////

        /*! \brief "Writes" to a meta
        
            This is the base for a writer to a meta.
        */
        class Meta::Writer {
        public:
        
            Writer&     alias(const char zAlias[]);
            Writer&     description(const char zDescription[]);
            Writer&     label(const char zLabel[]);
            Writer&     tag(const char zKey[]); // implies boolean TRUE
            Writer&     tag(const char zKey[], Variant&& value);
            Writer&     tag(const char zKey[], const Variant& value);
            Writer&     tls();  //!< Marks the TLS flag ... only meaningful on static variables, still
        
        protected:
                //  In proper usage, should never be null, however, guard!
            Meta* const m_meta = nullptr;
            
            Writer(Meta* myMeta) : m_meta(myMeta) 
            {
                assert(myMeta);
                assert(thread_safe_write());
            }
        };

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  GETTERS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////

        /*! \brief Abstract PropGetter
            
            This is an abstract "getter" that may or may not have an object associated with it.
        */
        class PropGetter : public Meta {
        public:
        
            /*! \brief "Gets" the value for the meta
            
                \param[out] dst     Pointer to destination, assumed to be correct buffer.
                \param[in] obj      Pointer to the object, if used.  (Ignored on static properties.)
            */
            virtual bool            get(void*dst, const void*obj) const = 0;
            
            /*! \brief Object/Type meta type expected for src
            
                \note This will be invalid() for static properties.
            */
            virtual const Meta&     object() const = 0;
            
            /*! \brief Object/Type for the data
            */
            virtual const Meta&     data() const = 0;
            
            /*! \brief Property this is associated with.
            */
            const PropertyInfo* property() const;

        protected:
            PropGetter(PropertyInfo*);
        };
        
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  PROP_GETTER IMPLEMENTATION
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        
            /*! \brief Abstract static/global getter of type
            
                This is an abstract getter bound to a specific data type.  Derived getters from this class only
                need to implement the "GET" routine and the constructor.
                
                \tparam T   data type being bound to
            */
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
                StaticPropGetter(PropertyInfo* propInfo) : PropGetter(propInfo) 
                {
                }
            };
            
            template <typename T>
            class XPV_PropGetter : public StaticPropGetter<T> {
            public:
                typedef const T* P;
                
                XPV_PropGetter(PropertyInfo*propInfo, P pointer) : StaticPropGetter<T>(propInfo), m_data(pointer) 
                {
                    assert(pointer);
                }
            
                virtual bool            get(void*dst, const void*) const override
                {
                    assert(dst);
                    *(T*) dst   = *m_data;
                    return true;
                }
                
            private:
                P      m_data;
            };
            
            template <typename T>
            class XFV_PropGetter : public StaticPropGetter<T> {
            public:
                typedef T (*FN)();
                XFV_PropGetter(PropertyInfo* propInfo, FN function) : StaticPropGetter<T>(propInfo), m_function(function) 
                {
                    assert(function);
                }
                
                virtual bool    get(void* dst, const void*) const  override
                {
                    assert(dst);
                    *(T*) dst   = std::move(m_function());
                    return true;
                }

            private:
                FN      m_function;
            };
            
            template <typename T>
            class XFR_PropGetter : public StaticPropGetter<T> {
            public:
                typedef T& (*FN)();
                XFR_PropGetter(PropertyInfo* propInfo, FN function) : StaticPropGetter<T>(propInfo), m_function(function) 
                {
                    assert(function);
                }
                
                virtual bool    get(void* dst, const void*) const  override
                {
                    assert(dst);
                    *(T*) dst   = std::move(m_function());
                    return true;
                }

            private:
                FN      m_function;
            };

            template <typename T>
            class XFCR_PropGetter : public StaticPropGetter<T> {
            public:
                typedef const T& (*FN)();
                XFCR_PropGetter(PropertyInfo* propInfo, FN function) : StaticPropGetter<T>(propInfo), m_function(function) 
                {
                    assert(function);
                }
                
                virtual bool    get(void* dst, const void*) const  override
                {
                    assert(dst);
                    *(T*) dst   = std::move(m_function());
                    return true;
                }

            private:
                FN      m_function;
            };

            template <typename T>
            class XFVR_PropGetter : public StaticPropGetter<T> {
            public:
                typedef void (*FN)(const T&);
                XFVR_PropGetter(PropertyInfo* propInfo, FN function) : StaticPropGetter<T>(propInfo), m_function(function)
                {
                    assert(function);
                }
                
                virtual bool    get(void* dst, const void*) const override
                {
                    assert(dst);
                    m_function(*(T*) dst);
                    return true;
                }

            private:
                FN      m_function;
            };
            
            template <typename T>
            class XFBR_PropGetter : public StaticPropGetter<T> {
            public:
                typedef bool (*FN)(const T&);
                XFBR_PropGetter(PropertyInfo* propInfo, FN function) : StaticPropGetter<T>(propInfo), m_function(function)
                {
                    assert(function);
                }
                
                virtual bool    get(void* dst, const void*) const override
                {
                    assert(dst);
                    return m_function(*(T*) dst);
                }
            private:
                FN      m_function;
            };
            
                //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            
            /*! \brief Abstract getter of type on class/struct
            
                This is an abstract getter bound to a specific data type/class.  Derived getters from this class only
                need to implement the "GET" routine and the constructor.

                \tparam T   variable data type
                \tparam C   bound class/struct's meta info
            */
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
                DynamicPropGetter(PropertyInfo* propInfo) : PropGetter(propInfo) 
                {
                }
            };
            
        
            /* !\brief PropGetter for a member variable
            
                PropGetter for the scenario of a member variable on an object.
            */
            template <typename C, typename T>
            class IPM_PropGetter : public DynamicPropGetter<C,T> {
            public:
                typedef const T (C::*P);
                IPM_PropGetter(PropertyInfo* propInfo, P pointer) : DynamicPropGetter<C,T>(propInfo), m_data(pointer) 
                {
                    assert(pointer);
                }

                virtual bool            get(void*dst, const void*obj) const override
                {
                    assert(dst);
                    assert(obj);
                    *(T*) dst   = *(((C*) obj)->*m_data);
                    return true;
                }
                
            private:
                P       m_data;
            };
            
            template <typename C, typename T>
            class IFV_PropGetter : public DynamicPropGetter<C,T> {
            public:
                typedef T (C::*FN)() const;
                IFV_PropGetter(PropertyInfo* propInfo, FN function) : DynamicPropGetter<C,T>(propInfo), m_function(function)
                {
                    assert(function);
                }
                
                virtual bool        get(void* dst, const void* obj) const override
                {
                    assert(dst);
                    assert(obj);
                    *(T*) dst   = (((const C*) obj)->*m_function)();
                    return true;
                }
                
            private:
                FN      m_function;
            };
            
            
            template <typename C, typename T>
            class IFR_PropGetter : public DynamicPropGetter<C,T> {
            public:
                typedef const T& (C::*FN)() const;
                IFR_PropGetter(PropertyInfo* propInfo, FN function) : DynamicPropGetter<C,T>(propInfo), m_function(function)
                {
                    assert(function);
                }
                
                virtual bool        get(void* dst, const void* obj) const override
                {
                    assert(dst);
                    assert(obj);
                    *(T*) dst   = (((const C*) obj)->*m_function)();
                    return true;
                }
                
            private:
                FN      m_function;
            };
        

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  SETTERS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////


        /*! \brief Abstract PropGetter
            
            This is an abstract "setter" that may or may not have an object associated with it.
        */
        class PropSetter : public Meta {
        public:
        
            /*! \brief "Sets" a property
            
                \param[out] obj     Pointer to object to set, can be null on a static object
                \param[in] value    Pointer to value to use on set, assumed to match data()
            */
            virtual bool            set(void*obj, const void*value) const = 0;
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
            StaticPropSetter(PropertyInfo*propInfo) : PropSetter(propInfo) 
            {
            }
        };
        
        template <typename T>
        class XPV_PropSetter : public StaticPropSetter<T> {
        public:
            typedef T* P;
            XPV_PropSetter(PropertyInfo* propInfo, P pointer) : StaticPropSetter<T>(propInfo), m_data(pointer) 
            {
                assert(pointer);
            }
            
            virtual bool            set(void*, const void*value) const override
            {
                assert(value);
                *m_data = *(const T*) value;
                return true;
            }
            

        private:
            P      m_data;
        };

        template <typename T>
        class XFV_PropSetter : public StaticPropSetter<T> {
        public:
            typedef void (*FN)(T);
            XFV_PropSetter(PropertyInfo* propInfo, FN function) : StaticPropSetter<T>(propInfo), m_function(function) 
            {
                assert(function);
            }
            
            virtual bool            set(void*, const void*value) const override
            {
                assert(value);
                (*m_function)(*(const T*) value);
                return true;
            }
            

        private:
            FN      m_function;
        };

        template <typename T>
        class XFBV_PropSetter : public StaticPropSetter<T> {
        public:
            typedef bool (*FN)(T);
            XFBV_PropSetter(PropertyInfo* propInfo, FN function) : StaticPropSetter<T>(propInfo), m_function(function) 
            {
                assert(function);
            }
            
            virtual bool            set(void*, const void*value) const override
            {
                assert(value);
                return (*m_function)(*(const T*) value);
            }
            

        private:
            FN      m_function;
        };

        template <typename T>
        class XFR_PropSetter : public StaticPropSetter<T> {
        public:
            typedef void (*FN)(const T&);
            XFR_PropSetter(PropertyInfo* propInfo, FN function) : StaticPropSetter<T>(propInfo), m_function(function) 
            {
                assert(function);
            }
            
            virtual bool            set(void*, const void*value) const override
            {
                assert(value);
                (*m_function)(*(const T*) value);
                return true;
            }
            

        private:
            FN      m_function;
        };


        template <typename T>
        class XFBR_PropSetter : public StaticPropSetter<T> {
        public:
            typedef bool (*FN)(const T&);
            XFBR_PropSetter(PropertyInfo* propInfo, FN function) : StaticPropSetter<T>(propInfo), m_function(function) 
            {
                assert(function);
            }
            
            virtual bool            set(void*, const void*value) const override
            {
                assert(value);
                return (*m_function)(*(const T*) value);
            }
            

        private:
            FN      m_function;
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
            DynamicPropSetter(PropertyInfo* propInfo) : PropSetter(propInfo) {}
        };

        
        template <typename C, typename T>
        class IPM_PropSetter : public DynamicPropSetter<C,T> {
        public:
            typedef T*(C::*P);
            IPM_PropSetter(PropertyInfo* propInfo, P pointer) : DynamicPropSetter<C,T>(propInfo), m_data(pointer) 
            {
                assert(pointer);
            }
            
            virtual bool            set(void*obj, const void*value) const override
            {
                assert(obj);
                assert(value);
                *(((C*) obj)->*m_data) =  *(const T*) value;
                return true;
            }
        
            
        private:
            P      m_data;
        };
        
        template <typename C, typename T>
        class IFV_PropSetter : public DynamicPropSetter<C,T>{
        public:
            typedef void (C::*FN)(T);
            
            IFV_PropSetter(PropertyInfo* propInfo, FN function) : DynamicPropSetter<C,T>(propInfo), m_function(function) 
            {
                assert(function);
            }
            
            virtual bool            set(void* obj, const void* value) const override
            {
                assert(obj);
                assert(value);
                (((C*) obj)->*m_function)(*(const T*) value);
                return true;
            }
            
        private:
            FN      m_function;
        };

        template <typename C, typename T>
        class IFBV_PropSetter : public DynamicPropSetter<C,T>{
        public:
            typedef bool (C::*FN)(T);
            
            IFBV_PropSetter(PropertyInfo* propInfo, FN function) : DynamicPropSetter<C,T>(propInfo), m_function(function) 
            {
                assert(function);
            }
            
            virtual bool            set(void* obj, const void* value) const override
            {
                assert(obj);
                assert(value);
                return (((C*) obj)->*m_function)(*(const T*) value);
            }
            
        private:
            FN      m_function;
        };

        template <typename C, typename T>
        class IFR_PropSetter : public DynamicPropSetter<C,T>{
        public:
            typedef void (C::*FN)(const T&);
            
            IFR_PropSetter(PropertyInfo* propInfo, FN function) : DynamicPropSetter<C,T>(propInfo), m_function(function) 
            {
                assert(function);
            }
            
            virtual bool            set(void* obj, const void* value) const override
            {
                assert(obj);
                assert(value);
                
                (((C*) obj)->*m_function)(*(const T*) value);
                return true;
            }
            
        private:
            FN      m_function;
        };

        template <typename C, typename T>
        class IFBR_PropSetter : public DynamicPropSetter<C,T>{
        public:
            typedef bool (C::*FN)(const T&);
            
            IFBR_PropSetter(PropertyInfo* propInfo, FN function) : DynamicPropSetter<C,T>(propInfo), m_function(function) 
            {
                assert(function);
            }
            
            virtual bool            set(void* obj, const void* value) const override
            {
                assert(obj);
                assert(value);
                return (((C*) obj)->*m_function)(*(const T*) value);
            }
            
        private:
            FN      m_function;
        };

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  PROPERTIES
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////


        template <typename T>
        class PropertyInfo::Writer : public Meta::Writer {
        public:
            Writer&  defValueInfo(const T& val)
            {
                assert(Meta::Writer::m_meta);
                assert(thread_safe_write());
                static_cast<PropertyInfo*>(m_meta) -> m_default.reference<T>() = val;
                return *this;
            }
        };
        
     //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


        template <typename T>
        class PropertyInfo::VarW : public Writer<T> {
        public:
        
            Writer<T>&     setter(void(*function)(T))
            {
                assert(function);
                assert(Meta::Writer::m_meta);

                new XFV_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
                return *this;
            }
            
            Writer<T>&     setter(void(*function)(const T&))
            {
                assert(function);
                assert(Meta::Writer::m_meta);
                assert(thread_safe_write());
                new XFR_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
                return *this;
            }
            
            Writer<T>&     setter(bool(*function)(T))
            {
                assert(function);
                assert(Meta::Writer::m_meta);
                assert(thread_safe_write());
                new XFBV_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
                return *this;
            }
            
            Writer<T>&     setter(bool(*function)(const T&))
            {
                assert(function);
                assert(Meta::Writer::m_meta);
                assert(thread_safe_write());
                new XFBR_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
                return *this;
            }
        };
    
        template <typename C, typename T>
        class PropertyInfo::PropW : public Writer<T> {
        public:
        
            Writer<T>&     setter(void (C::*function)(T))
            {
                assert(function);
                assert(Meta::Writer::m_meta);
                assert(thread_safe_write());
                new IFV_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
                return *this;
            }
            
            Writer<T>&     setter(void (C::*function)(const T&))
            {
                assert(function);
                assert(Meta::Writer::m_meta);
                assert(thread_safe_write());
                new IFR_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
                return *this;
            }
            
            Writer<T>&     setter(bool (C::*function)(T))
            {
                assert(function);
                assert(Meta::Writer::m_meta);
                assert(thread_safe_write());
                new IFBV_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
                return *this;
            }
            
            Writer<T>&     setter(bool (C::*function)(const T&))
            {
                assert(function);
                assert(Meta::Writer::m_meta);
                assert(thread_safe_write());
                new IFBR_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
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
            PropertyInfo::Writer<T>       variable(const char szName[], T* pointer)
            {
                assert(pointer);
                PropertyInfo*ret  = new PropertyInfo(szName, meta<T>(), m_meta, STATIC);
                new XPV_PropGetter<T>(ret, pointer);
                new XPV_PropSetter<T>(ret, pointer);
                return PropertyInfo::Writer<T>{ret};
            }

            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read/write.
                
                \tparam T   type
            */
            template <typename T>
            PropertyInfo::Writer<T>       variable(const char szName[], const T* pointer)
            {
                assert(pointer);
                PropertyInfo*ret  = new PropertyInfo(szName, meta<T>(), m_meta, STATIC);
                new XPV_PropGetter<T>(ret, pointer);
                return PropertyInfo::Writer<T>{ret};
            }

            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read only.
                
                \tparam T   type
            */
            template <typename T>
            PropertyInfo::VarW<T>           variable(const char szName[], T (*function)())
            {
                assert(function);
                PropertyInfo*   ret = new PropertyInfo(szName, meta<T>(), m_meta, STATIC);
                new XFV_PropGetter<T>(ret, function);
                return PropertyInfo::VarW<T>(ret);
            }

            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read only.
                
                \tparam T   type
            */
            template <typename T>
            PropertyInfo::VarW<T>           variable(const char szName[], const T& (*function)())
            {
                assert(function);
                PropertyInfo*   ret = new PropertyInfo(szName, meta<T>(), m_meta, STATIC);
                new XFCR_PropGetter<T>(ret, function);
                return PropertyInfo::VarW<T>(ret);
            }

            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read only.
                
                \tparam T   type
            */
            template <typename T>
            PropertyInfo::VarW<T>           variable(const char szName[], void (*function)(T&))
            {
                assert(function);
                PropertyInfo*   ret = new PropertyInfo(szName, meta<T>(), m_meta, STATIC);
                new XFVR_PropGetter<T>(ret, function);
                return PropertyInfo::VarW<T>(ret);
            }
            

            /*! \brief Defines a global variable
            
                This defines a GLOBAL variable for the given scope, read only.
                
                \tparam T   type
            */
            template <typename T>
            PropertyInfo::VarW<T>           variable(const char szName[], bool (*function)(T&))
            {
                assert(function);
                PropertyInfo*   ret = new PropertyInfo(szName, meta<T>(), m_meta, STATIC);
                new XFBR_PropGetter<T>(ret, function);
                return PropertyInfo::VarW<T>(ret);
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
            Static( CompoundInfo* compound ) : Meta::Writer(compound) {}

        };
        
        template <typename C>
        class CompoundInfo::Dynamic : public CompoundInfo::Static {
        public:
        
            /*! \brief Defines a property
            
                This defines a property for the type/object
            
                \tparam T       type
                \param  pointer Pointer to class/type member
            */
            template <typename T>
            PropertyInfo::Writer<T>     property(const char szName[], T (C::*pointer))
            {
                assert(pointer);
                PropertyInfo*ret  = new PropertyInfo(szName, meta<T>(), m_meta, STATE);
                new IPM_PropGetter<C,T>(ret, pointer);
                new IPM_PropSetter<C,T>(ret, pointer);
                return PropertyInfo::Writer<T>{ret};
            }
            
            /*! \brief Defines a property
            
                This defines a property for the type/object
            
                \tparam T       type
                \param  pointer Pointer to class/type member
            */
            template <typename T>
            PropertyInfo::Writer<T>     property(const char szName[], const T (C::*pointer))
            {
                assert(pointer);
                PropertyInfo*ret  = new PropertyInfo(szName, meta<T>(), m_meta, STATE);
                new IPM_PropGetter<C,T>(ret, pointer);
                return PropertyInfo::Writer<T>{ret};
            }

            /*! \brief Defines a property
            
                This defines a property for the type/object
            
                \tparam T       type
                \param  p       Function pointer to getter (const & returns)
            */
            template <typename T>
            PropertyInfo::PropW<C,T>    property(const char szName[], T (C::*function)() const)
            {
                assert(function);
                PropertyInfo*ret  = new PropertyInfo(szName, meta<T>(), m_meta);
                new IFV_PropGetter<C,T>(ret, function);
                return PropertyInfo::PropW<C,T>{ret};
            }
            
            /*! \brief Defines a property
            
                This defines a property for the type/object
            
                \tparam T       type
                \param  p       Function pointer to getter (const & returns)
            */
            template <typename T>
            PropertyInfo::PropW<C,T>    property(const char szName[], const T& (C::*function)() const)
            {
                assert(function);
                PropertyInfo*ret  = new PropertyInfo(szName, meta<T>(), m_meta);
                new IFR_PropGetter<C,T>(ret, function);
                return PropertyInfo::PropW<C,T>{ret};
            }


            template <typename T>
            PropertyInfo::PropW<C,T>    property(const char szName[], void (C::*function)(T&) const);
        
            template <typename T>
            PropertyInfo::PropW<C,T>    property(const char szName[], bool (C::*function)(T&) const);
            
            template <typename ... Args>
            MethodInfo::Writer          method(const char szName[], void (C::*function)(Args...));
            
            template <typename ... Args>
            MethodInfo::Writer          method(const char szName[], void (C::*function)(Args...) const);

            template <typename T, typename ... Args>
            MethodInfo::Writer          method(const char szName[], T (C::*function)(Args...));
            
            template <typename T, typename ... Args>
            MethodInfo::Writer          method(const char szName[], T (C::*function)(Args...) const);
            
            
        protected:
            Dynamic(CompoundInfo* c) : Static(c) {}
        };


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  GLOBAL
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    class GlobalInfo::Writer : public CompoundInfo::Static {
    public:
    
        Writer(GlobalInfo* p) : CompoundInfo::Static(p) 
        {
            assert(p);
        }
    };

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  OBJECT
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    template <typename C>
    class ObjectInfo::Writer : public CompoundInfo::Dynamic<C> {
    public:
    
        /*! \brief Sets the base object (if not already set)
        */
        template <typename B>
        Writer&     base()
        {
            static_assert( std::is_base_of_v<B, C>, "T must derive from B!" );
            ObjectInfo*     obj = static_cast<ObjectInfo*>(Meta::Writer::m_meta);
            assert(!obj->m_base);
            obj->m_base = &meta<B>();
            return *this; 
        }
        
        Writer(ObjectInfo* obj) : CompoundInfo::Dynamic<C>(obj) 
        {
            assert(obj);
        }
        
        Writer(ObjectInfo& obj) : CompoundInfo::Dynamic<C>(&obj) 
        {
        }
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
    
    /*! \brief Gathers Types
        
        This gathers the template parameter type as a type info into the vector, IF meta type aware.
        \return One always one for this specialization of the function.
    */
    template <typename T>
    unsigned    gather_types(Vector<const TypeInfo*>& ret)
    {
        if constexpr (is_type_v<T>){
            ret << &meta<T>();
        } 
        // TODO pointers, references, shared_ptrs, etc to known types.
        return 1;
    }
    
    /*! \brief Gathers types from parameter pack
    
        This breaks down a template parameter pack for its type infos
    */
    template <typename T, typename... Args>
    unsigned gather_types(Vector<const TypeInfo*>& ret)
    {
        gather_types<T>(ret);
        return 1 + gather_types<Args...>(ret);
    }
    
    template <typename> struct GatherTemplateArgs { 
        unsigned operator()(Vector<const TypeInfo*>& ret)
        {
            return 0;
        }
    };
    
    template <template <typename...> class Tmpl, typename ...Args> 
    struct GatherTemplateArgs<Tmpl<Args...>> {
        unsigned    operator()(Vector<const TypeInfo*>& ret) 
        {
            return gather_types<Args...>(ret);
        }
    };

    template <typename T>
    class TypeInfo::Typed : public type_info_t<T> {
    protected:
        Typed(const char* zName, const char* zFile, id_t i=AUTO_ID) : type_info_t<T>(zName, zFile, i)
        {
            options_t   opts    = 0;
        
            TypeInfo::m_default.ctorCopy(T{});
            TypeInfo::m_copyB         = [](DataBlock& dst, const DataBlock&src){
                dst.reference<T>() = src.reference<T>();
            };
            TypeInfo::m_copyR        = [](DataBlock& dst, const void* src){
                dst.reference<T>() = *(const T*) src;
            };
            TypeInfo::m_ctorCopyR     = [](DataBlock& dst, const void* src){ 
                dst.ctorCopy(*(const T*) src); 
            };
            TypeInfo::m_ctorCopyB     = [](DataBlock& dst, const DataBlock& src){ 
                dst.ctorCopy( src.reference<T>());
            };
            TypeInfo::m_ctorMove      = [](DataBlock& dst, DataBlock&& src){
                if constexpr (sizeof(T) <= sizeof(DataBlock))
                    dst.ctorMove( std::move(src.reference<T>()));
                else
                    dst.Pointer = src.Pointer;
            };
            TypeInfo::m_dtor          = [](DataBlock& tgt)
            {
                tgt.dtor<T>();
            };
            TypeInfo::m_equal         = [](const DataBlock& a, const DataBlock& b) -> bool 
            {
                return a.reference<T>() == b.reference<T>();
            };
            
            if constexpr (has_less_v<T>){
                TypeInfo::m_less      = [](const DataBlock& a, const DataBlock& b) -> bool 
                {
                    return a.reference<T>() < b.reference<T>();
                };
                opts |= LESS;
            }
            
            TypeInfo::m_moveB         = [](DataBlock& a, DataBlock&&b) 
            {
                a.reference<T>() = std::move( b.reference<T>());
            };
            
            TypeInfo::m_size          = sizeof(T);
            
            if constexpr ( is_template_v<T>) {
                TypeInfo::m_template.params     = GatherTemplateArgs<T>(TypeInfo::m_template.args);
                if(!TypeInfo::m_template.args.empty())  // only flag it as a template if any parameters trigger
                    opts |= TEMPLATE;
            }
            
            if constexpr ( has_stream_v<T>) {
                TypeInfo::m_print   = TypeInfo::m_write = [](Stream& dst, const void* src){
                    dst << *(const T*) src;
                };
            }
            
            if(is_small_v<T>)
                opts |= SMALL;
            if(opts)
                Meta::set_options(opts);
        }
    };
    
    template <typename T>
    class TypeInfo::Special : public Typed<T> {
    protected:
        Special(const char* zName, const char* zFile, id_t i=AUTO_ID) : Typed<T>(zName, zFile, i) {}
    };
    
    template <typename K, typename V>
    class TypeInfo::Special<Hash<K,V>> : public Typed<Hash<K,V>> {
    protected:
        Special(const char* zName, const char* zFile, id_t i=AUTO_ID) : Typed<Hash<K,V>>(zName, zFile, i)
        {
            Meta::set_option(COLLECTION);
        }
    };

    template <typename T>
    class TypeInfo::Special<List<T>> : public Typed<List<T>> {
    protected:
        Special(const char* zName, const char* zFile, id_t i=AUTO_ID) : Typed<List<T>>(zName, zFile, i) 
        {
            Meta::set_option(COLLECTION);
        }
    };
    
    template <typename K, typename V, typename C>
    class TypeInfo::Special<Map<K,V,C>> : public Typed<Map<K,V,C>> {
    protected:
        Special(const char* zName, const char* zFile, id_t i=AUTO_ID) : Typed<Map<K,V,C>>(zName, zFile, i)
        {
            Meta::set_option(COLLECTION);
        }
    };

    template <typename K, typename V, typename C>
    class TypeInfo::Special<MultiMap<K,V,C>> : public Typed<MultiMap<K,V,C>> {
    protected:
        Special(const char* zName, const char* zFile, id_t i=AUTO_ID) : Typed<MultiMap<K,V,C>>(zName, zFile, i)
        {
            Meta::set_option(COLLECTION);
        }
    };

    template <typename T, typename C>
    class TypeInfo::Special<Set<T,C>> : public Typed<Set<T,C>> {
    protected:
        Special(const char* zName, const char* zFile, id_t i=AUTO_ID) : Typed<Set<T,C>>(zName, zFile, i) 
        {
            Meta::set_option(COLLECTION);
        }
    };

    template <typename T>
    class TypeInfo::Special<Vector<T>> : public Typed<Vector<T>> {
    protected:
        Special(const char* zName, const char* zFile, id_t i=AUTO_ID) : Typed<Vector<T>>(zName, zFile, i) 
        {
            Meta::set_option(COLLECTION);
        }
    };
    

    //! The FINAL storage class, the one that's generic
    template <typename T>
    class TypeInfo::Final : public Special<T> {
    private:
        friend class InfoBinder<T>;
        Final(const char* zName, const char* zFile, id_t i=AUTO_ID) : Special<T>(zName, zFile, i) {}
        static TypeInfo&       s_save;
    };


    template <typename T>
    using TypeInfoWriterBase    = std::conditional<std::is_class_v<T>, CompoundInfo::Dynamic<T>, Meta::Writer>::type;

    template <typename T>
    class TypeInfo::Writer : public TypeInfoWriterBase<T> {
    public:
        static_assert( InfoBinder<T>::IsType, "T must be meta-type declared!");
    
        template <typename U>
        requires std::is_nothrow_convertible_v<U,T>
        void casts()
        {
            static_assert( is_type_v<U>, "U must be meta-type declared!");
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_convert[ &InfoBinder<U>::bind()] = [](void* dst, const void* src){
                    *(U*) dst = U( *(const T*) src);
                };
            }
        }
        
        template <typename U, U(*FN)(T)>
        void converts()
        {
            static_assert(is_type_v<U>, "U must be meta-type decalred!");
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_convert[ &InfoBinder<U>::bind()] = [](void* dst, const void* src){
                    *(U*) dst = FN( *(const T*) src);
                };
            }
        }
        
        template <typename U, U(*FN)(const T&)>
        void converts()
        {
            static_assert(is_type_v<U>, "U must be meta-type decalred!");
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_convert[ &InfoBinder<U>::bind()] = [](void* dst, const void* src){
                    *(U*) dst = FN( *(const T*) src);
                };
            }
        }

        template <typename U, void (*FN)(U&, const T&)>
        void converts()
        {
            static_assert(is_type_v<U>, "U must be meta-type decalred!");
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_convert[ &InfoBinder<U>::bind()] = [](void* dst, const void* src){
                    FN(*(U*) dst,  *(const T*) src);
                };
            }
        }
        
        template <String(*FN)(T)>
        void    format()
        {
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_write   = [](Stream& dst, const void* src)  {
                    dst << FN(*(const T*) src);
                };
            }
        }
        
        template <String(*FN)(const T&)>
        void    format()
        {
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_write   = [](Stream& dst, const void* src)  {
                    dst << FN(*(const T*) src);
                };
            }
        }
        
        
        template <bool (*FN)(T&, const std::string_view&)>
        void    parse()
        {
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_parse     = [](void* dst, const std::string_view&src) -> bool {
                    return FN(*(T*) dst, src);
                };
            }
        }

        template <bool (*FN)(const std::string_view&, T&)>
        void    parse()
        {
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_parse     = [](void* dst, const std::string_view&src) -> bool {
                    return FN(src, *(T*) dst);
                };
            }
        }

        template <Result<T> (*FN)(const std::string_view&)>
        void    parse()
        {
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_parse     = [](void* dst, const std::string_view&src) -> bool {
                    Result<T>   r   = FN(src);
                    *(T*) dst = std::move(r.value);
                    return r.good;
                };
            }
        }

        template <String(*FN)(T)>
        void    print()
        {
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_print   = [](Stream& dst, const void* src)  {
                    dst << FN(*(const T*) src);
                };
            }
        }
        
        template <String(*FN)(const T&)>
        void    print()
        {
            if(thread_safe_write()){
                static_cast<TypeInfo*>(Meta::Writer::m_meta)->m_print   = [](Stream& dst, const void* src)  {
                    dst << FN(*(const T*) src);
                };
            }
        }

        Writer(TypeInfo* ti) : TypeInfoWriterBase<T>(ti) {}
        Writer(TypeInfo& ti) : TypeInfoWriterBase<T>(&ti) {}
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