#pragma once
#include "CompoundInfo.hpp"
#include <util/collection/Hash.hpp>


namespace yq {


    /* \brief Declares a type that can be stored unobtrusively
    */
    class TypeInfo : public CompoundInfo {
        friend class ::yq::Variant;
    public:
    
        static const Vector<const TypeInfo*>    all();
        static const TypeInfo*                  lookup(id_t);
        static const TypeInfo*                  lookup(const std::string_view&);
    
    
        bool    is_small() const { return static_cast<bool>(flags() & SMALL); }
    
        virtual const char*     generic() const override { return "Type"; }
        
        template <typename T> class Writer;
        template <typename T> class Typed;      //  base type
        template <typename T> class Special;    //  allows for specialization
        template <typename T> class Final;      //  Final one for storing
        
    protected:
        TypeInfo(const char* zName, const char* zFile, id_t i=AUTO_ID);
        virtual ~TypeInfo();

        virtual void    sweep_impl() override;

        typedef void        (*FNConvert)(void*, const void*);
        typedef void        (*FNCopyBlkBlk)(DataBlock&, const DataBlock&);

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
        FNFormat                m_format        = nullptr;
        FNParse                 m_parse         = nullptr;
        
    };
    
    /*! \brief Converts meta to type, if it's valid
    
        \return TypeInfo pointer, if valid, NULL otherwise
    */
    const TypeInfo* to_type(const Meta* m);
    
}
