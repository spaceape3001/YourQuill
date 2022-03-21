////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "MethodInfo.hpp"
#include "PropertyInfo.hpp"

namespace yq {

    /*! \brief Intermediate Meta that can have methods/properties.  
    
        This is here to facilitate sharing method/properties between both types & objects & global.
    
    */
    class CompoundInfo : public Meta {
    public:

        class Static;

        template <typename C> class Dynamic;
    
        const auto&             methods() const { return m_methods; }
        const auto&             properties() const { return m_properties; }
        const std::string_view& file() const { return m_file; }
    
    protected:
        friend class Global;
        
        LUC<MethodInfo>         m_methods;
        LUC<PropertyInfo>       m_properties;
        std::string_view        m_file;
        
        CompoundInfo(std::string_view zName, const char zFile[], Meta* par=nullptr, id_t i=AUTO_ID);
        ~CompoundInfo();

        virtual void            sweep_impl() override;
        
        void        gather(LUC<MethodInfo>&);
        void        gather(LUC<PropertyInfo>&);

    };
}
