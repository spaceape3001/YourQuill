////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string_view>
#include <basic/Stack.hpp>


namespace yq {
    class Stream;
    
    struct MarkdownConfig;
    
    class Markdown {
    public:
    
        struct Config;

        Markdown(Stream& s, const Config&);
        ~Markdown();
        
        /*! \brief Processes the markdown data, pushing it into stream
            \note Limitation is this assumes complete lines per call
            \return TRUE if the whole thing was processed into the output
        */
        void            process(std::string_view);
        
    private:
        
    
            // match detector, line is adjusted to match what's needed for apply
        typedef bool    (Markdown::*FNBlockMatch)(std::string_view&);
        
            // applies the match
        typedef void    (Markdown::*FNBlockApply)(std::string_view);
        
            // IF it's mullti line, this is present
        typedef bool    (Markdown::*FNBlockLine)(std::string_view);
        
            // This will finish a multi-line (if needed)
        typedef void    (Markdown::*FNBlockFinish)();
        
        
        struct Block {
            FNBlockMatch    match   = nullptr;
            FNBlockApply    apply   = nullptr;
            FNBlockLine     line    = nullptr;
            FNBlockFinish   finish  = nullptr;
            
            bool is_multiline() const { return static_cast<bool>(line); }
        };

        struct Block;
        
        bool    code_block_match(std::string_view&);
        void    code_block_apply(std::string_view);
        bool    code_block_line(std::string_view);
        void    code_block_finish();
        
        bool    header_line_match(std::string_view&);
        void    header_line_apply(std::string_view);
        
        bool    horz_rule_match(std::string_view&);
        void    horz_rule_apply(std::string_view);
        
        bool    quote_match(std::string_view&);
        void    quote_apply(std::string_view);
        bool    quote_line(std::string_view);
        void    quote_finish();
        

        bool    push_block(std::string_view);
        void    pop_block();
        void    parse_line(std::string_view);
        
        Stream&                 m_stream;
        const Config&           m_config;
        Stack<const Block*>     m_stack;
        
            //  used by blocks to carry things over....
        size_t                  m_local0;
        
        static const    Block  s_blocks[];
    };
    
    struct Markdown::Config {
        unsigned int    tab_width           = 4;
        bool            emphasis_enabled    = true;
        bool            html_paragraph      = true;
    };
    
}

