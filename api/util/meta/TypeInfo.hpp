#pragma once
#include "CompoundInfo.hpp"
#include <util/collection/Hash.hpp>


namespace yq {
    namespace meta {

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
            template <typename T> class Typed;
            
        protected:
            TypeInfo(const char* zName, const char* zFile, id_t i=AUTO_ID);

            virtual void    sweep_impl() override;

            typedef void        (*FNConvert)(void*, const void*);
            typedef void        (*FNCopyBlkBlk)(impl::DataBlock&, const impl::DataBlock&);

                //  COPY CONSTRUCTORS
            typedef void        (*FNCtorCopyRawBlk)(impl::DataBlock&, const void*);
            typedef void        (*FNCtorCopyBlkBlk)(impl::DataBlock&, const impl::DataBlock&);

                //  MOVE CONSTRUCTORS
            typedef void        (*FNCtorMove)(impl::DataBlock&, impl::DataBlock&&);

                //  DESTRUCTOR
            typedef void        (*FNDtor)(impl::DataBlock&);

            typedef bool        (*FNEqual)(const impl::DataBlock&, const impl::DataBlock&);
            typedef void        (*FNMoveBlkBlk)(impl::DataBlock&, impl::DataBlock&&);
            typedef void        (*FNPrint)(Stream&, const void*);
            typedef String      (*FNFormat)(const void*);                       // TODO: REVIEW 
            typedef bool        (*FNParse)(void*, const std::string_view&);     // TODO: REVIEW 

        private:
        
            using ConvertHash   = Hash<const TypeInfo*, FNConvert>;
            friend class PropertyInfo;
            friend class MethodInfo;
        
            LUC<PropertyInfo>           m_properties;
            LUC<MethodInfo>             m_methods;
            impl::DataBlock         m_defCopy;
        protected:

            size_t                  m_size              = 0;
            ConvertHash             m_convert;
         
            FNCopyBlkBlk            m_copyB         = nullptr;
            FNCtorCopyRawBlk        m_ctorCopyR     = nullptr;
            FNCtorCopyBlkBlk        m_ctorCopyB     = nullptr;
            FNCtorMove              m_ctorMove      = nullptr;
            FNDtor                  m_dtor          = nullptr;
            FNEqual                 m_equal         = nullptr;
            FNMoveBlkBlk            m_moveB         = nullptr;
            FNPrint                 m_print         = nullptr;
            FNFormat                m_ioFormat      = nullptr;          // TODO: REVIEW
            FNParse                 m_ioParse       = nullptr;          // TODO: REVIEW
            
        };
        
        /*! \brief Converts meta to type, if it's valid
        
            \return TypeInfo pointer, if valid, NULL otherwise
        */
        const TypeInfo* to_type(const Meta* m);
        
        
    }

    template <typename T>
    auto    metaType()
    {
        static_assert(meta::is_type<T>, "Must be meta-type-declared!");
        return meta::Binder<T>::bind();
    }
}
