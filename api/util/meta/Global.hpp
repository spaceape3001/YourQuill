////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Meta.hpp"
#include "Binder.hpp"
#include "CompoundInfo.hpp"

namespace yq {

    //! \brief Global "CLASS"
    //! 
    //! \note this class is here for the API meta binding, not subsequently used, call namespace
    class Global { };
    
    namespace global {
        const StringSet&    functions();
        const StringSet&    variables();
    };

    namespace meta {
        class GlobalInfo : public CompoundInfo {
        public:

            const LUC<MethodInfo>&      methods() const { return m_methods; }
            const LUC<PropertyInfo>&    properties() const { return m_properties; }
            
            class Writer;
            
            static GlobalInfo&          instance();
    
        protected:
            GlobalInfo();
            virtual void                sweep_impl() override;

            friend class PropertyInfo;
            friend class MethodInfo;

        private:
            LUC<MethodInfo>             m_methods;
            LUC<PropertyInfo>           m_properties;
        };
        
        template <>
        struct Binder<Global> : public False {
            static constexpr const bool     Defined     = true;
            static constexpr const bool     IsCompound  = true;     
            static const GlobalInfo&        bind();
            static GlobalInfo::Writer       edit();
        };
    };
    
    
    
    
}

