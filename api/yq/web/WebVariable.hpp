////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/Meta.hpp>
#include <yq/meta/MetaWriter.hpp>

namespace yq {
    class Stream;
    struct WebContext;
    class WebVariable;
    
    using WebVarMap     = Map<std::string_view, const WebVariable*, IgCase>;
    
    namespace web {

        // Not thread-safe until freeze() has been called.
        const std::vector<const WebVariable*>& all_variables();

        const WebVariable*   variable(std::string_view);
        
        // Not thread-safe until freeze() has been called.
        const WebVarMap&    variable_map();
    }


    class WebVariable : public Meta {
    public:
    
        
        virtual void handle(Stream&, WebContext&) const = 0;
        
        class Writer;
    
    protected:
        WebVariable(std::string_view, const std::source_location&);
        ~WebVariable();

    private:
    };
    
    class WebVariable::Writer : public Meta::Writer {
    public:
        //! Sets the description
        Writer&  description(std::string_view);
    
    
        Writer(WebVariable*);
        Writer(Writer&&);
        Writer& operator=(Writer&&);
        ~Writer();
        
        operator const WebVariable* () const { return m_var; }
        
        Writer(const Writer&) = delete;
        Writer& operator=(const Writer&&) = delete;
    private:
        WebVariable*   m_var = nullptr;
    };

}
