#pragma once
#include "CompoundInfo.hpp"
#include <unordered_map>


namespace yq {
    /* \brief Declares a type that can be stored unobtrusively
    */
    class TypeInfo : public CompoundInfo {
        friend class Variant;
    public:
    
            // WARNING UNSAFE IN UNLOCKED MULTITHREADED MODE!
        static const Vector<const TypeInfo*>&   all();
        static const TypeInfo*                  lookup(id_t);
        static const TypeInfo*                  lookup(const std::string_view&);
    
        bool        can_parse() const { return m_parse ? true : false; }
        bool        can_write() const { return m_write ? true : false; }
        bool        can_print() const { return m_print ? true : false; }

        virtual const char*     generic() const override { return "Type"; }

        bool        is_small() const { return static_cast<bool>(flags() & SMALL); }

        size_t      size() const { return m_size; }
    
        
        template <typename T> class Writer;
        template <typename T> class Typed;      //  base type
        template <typename T> class Special;    //  allows for specialization
        template <typename T> class Final;      //  Final one for storing
        
    protected:
        TypeInfo(std::string_view zName, const std::source_location& sl, id_t i=AUTO_ID);
        virtual ~TypeInfo();

        virtual void    sweep_impl() override;

        typedef void        (*FNConvert)(void*, const void*);
        typedef void        (*FNCopyBlkBlk)(DataBlock&, const DataBlock&);
        typedef void        (*FNCopyRawBlk)(DataBlock&, const void*);

            //  COPY CONSTRUCTORS
        typedef void        (*FNCtorCopyRawBlk)(DataBlock&, const void*);
        typedef void        (*FNCtorCopyBlkBlk)(DataBlock&, const DataBlock&);

            //  MOVE CONSTRUCTORS
        typedef void        (*FNCtorMove)(DataBlock&, DataBlock&&);

            //  DESTRUCTOR
        typedef void        (*FNDtor)(DataBlock&);

        typedef bool        (*FNCompare)(const DataBlock&, const DataBlock&);
        typedef void        (*FNMoveBlkBlk)(DataBlock&, DataBlock&&);
        typedef void        (*FNFormat)(Stream&, const void*);
        typedef bool        (*FNParse)(void*, const std::string_view&);

    
        using ConvertHash   = Hash<const TypeInfo*, FNConvert>;
        friend class PropertyInfo;
        friend class MethodInfo;
        
    
        LUC<PropertyInfo>           m_properties;
        LUC<MethodInfo>             m_methods;
        DataBlock                   m_default;
        
        struct {
            Vector<const TypeInfo*> args;
            
            //!  total number of parameters (typed or not)
            unsigned                params          = 0;
        }                           m_template;

        size_t                  m_size              = 0;
        ConvertHash             m_convert;
        
        FNCopyBlkBlk            m_copyB         = nullptr;
        FNCopyRawBlk            m_copyR         = nullptr;
        FNCtorCopyRawBlk        m_ctorCopyR     = nullptr;
        FNCtorCopyBlkBlk        m_ctorCopyB     = nullptr;
        FNCtorMove              m_ctorMove      = nullptr;
        FNDtor                  m_dtor          = nullptr;
        FNCompare               m_equal         = nullptr;
        FNCompare               m_less          = nullptr;
        FNMoveBlkBlk            m_moveB         = nullptr;
            // Print is for user eyes
        FNFormat                m_print         = nullptr;
            // Format is for data storage
        FNFormat                m_write        = nullptr;
        FNParse                 m_parse         = nullptr;
        
    };
    
    /*! \brief Converts meta to type, if it's valid
    
        \return TypeInfo pointer, if valid, NULL otherwise
    */
    inline const TypeInfo* to_type(const Meta* m)
    {
        return (m && (m->flags() & TYPE)) ? static_cast<const TypeInfo*>(m) : nullptr;
    }
    
    /*! \brief Converts meta to type, if it's valid
    
        \return TypeInfo pointer, if valid, NULL otherwise
    */
    inline TypeInfo* to_type(Meta* m)
    {
        return (m && (m->flags() & TYPE)) ? static_cast<TypeInfo*>(m) : nullptr;
    }
}
