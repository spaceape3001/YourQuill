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

    class GlobalInfo : public CompoundInfo {
    public:

        const LUC<MethodInfo>&      methods() const { return m_methods; }
        const LUC<PropertyInfo>&    properties() const { return m_properties; }
        
        template <typename> class Writer;   // which will only ever by GlobalInfo...however, consistency
        
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
    struct InfoBinder<Global>  {
        static constexpr const bool     Defined     = true;
        static constexpr const bool     IsCompound  = true;     
        static const GlobalInfo&        bind();
        static GlobalInfo&              edit();
    };
    
    inline GlobalInfo*  to_global(Meta*m)
    {
        return (m && (m->flags() & GLOBAL)) ? static_cast<GlobalInfo*>(m) : nullptr;
    }
    
    inline const GlobalInfo*  to_global(const Meta*m)
    {
        return (m && (m->flags() & GLOBAL)) ? static_cast<const GlobalInfo*>(m) : nullptr;
    }
}

