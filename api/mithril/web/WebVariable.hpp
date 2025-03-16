////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/Meta.hpp>
#include <yq/meta/MetaWriter.hpp>
#include <mithril/typedef/metabase.hpp>

namespace yq {
    class Stream;
}

namespace yq::mithril {
    struct WebContext;
    class WebVariable;
    class WebHtml;
    
    using WebVarMap     = Map<std::string_view, const WebVariable*, IgCase>;
    
    namespace web {

        // Not thread-safe until freeze() has been called.
        const std::vector<const WebVariable*>& all_variables();

        const WebVariable*   variable(std::string_view);
        
        // Not thread-safe until freeze() has been called.
        const WebVarMap&    variable_map();
    }


    /*! \brief Variable for text substitution
    
        The default HTML output allows has a minimal level of text
        substitution (With {{ and }} ), this represents a registered
        variable for it, that allows for the text to be delayed 
        and computed at demand time.  (ie, it can change between invocations)
    */
    class WebVariable : public MetaBase {
    public:
    
        
        virtual void handle(WebHtml&) const = 0;
        
        class Writer;
    
    protected:
        WebVariable(std::string_view, const std::source_location&);
        ~WebVariable();

    private:
    };
    
    class WebVariable::Writer : public MetaBase::Writer {
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
