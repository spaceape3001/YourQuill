////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Markdown.hpp"
#include <yq/basic/Compare.hpp>
#include <0/io/StreamOps.hpp>
#include <yq/io/Stream.hpp>
#include <0/basic/TextUtils.hpp>

/*
    Crediting https://github.com/progsource/maddy for the guidance in how-to, however, the code
    style left a lot to be desired for the implementation I wanted.  
    
    Resolving syntax issues using Common Mark: https://spec.commonmark.org/0.30/
*/

using std::literals::string_view_literals::operator""sv;

namespace yq::mithril {
    void            html_escape_write(Stream& s, std::string_view);

    Markdown::Markdown(Stream& s, const Config&c) : m_stream(s), m_config(c)
    {
    }
    
    Markdown::~Markdown()
    {
        while(!m_stack.empty()){
            const Block*   blk = nullptr;
            m_stack >> blk;
            if(blk && blk->line){
                (this->*(blk->line))(std::string_view());
                if(blk->finish)
                    (this->*(blk->finish))();
            }
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    namespace {
        bool    spacy_starts(std::string_view text, size_t mx, std::initializer_list<std::string_view> vals)
        {
            size_t n = count_starts(text, ' ');
            if(n>3)
                return false;
            return is_in(text.substr(n), vals);
        }
    }
    
    
    bool    Markdown::code_block_match(std::string_view&line)
    {
        if(starts(line, "```"sv)){
            line    = line.substr(3);
            return true;
        }
        return false;
    }
    
    void    Markdown::code_block_apply(std::string_view line)
    {
        m_stream << "<pre><code>" << line << "\n";
    }
    
    bool    Markdown::code_block_line(std::string_view line)
    {
        if(line == "```"sv)
            return true;
            
        m_stream << line << '\n';
        return false;
    }
    
    void    Markdown::code_block_finish()
    {
        m_stream << "</pre></code>\n";
    }

    //  -----------------------

    bool    Markdown::header_line_match(std::string_view& line)
    {
        m_local0        = count_starts(line, '#');
        if(!is_between<size_t>(m_local0, 1ULL, 6ULL))
            return false;
        line    = trimmed(line.substr(m_local0));
        return true;
    }
    
    void    Markdown::header_line_apply(std::string_view line)
    {
        m_stream << "<H" << m_local0 << ">";
        html_escape_write(m_stream, line);
        m_stream << "</H" << m_local0 << ">\n";
    }

    //  -----------------------
    bool    Markdown::horz_rule_match(std::string_view&line)
    {
        return spacy_starts(line, 3, { "---", "***", "+++" });
    }
    
    void    Markdown::horz_rule_apply(std::string_view)
    {
        m_stream << "<HR />\n";
    }

    //  -----------------------
    bool    Markdown::quote_match(std::string_view&line)
    {
        size_t  n   = count_starts(line, ' ');
        if((n<=3) && (n<line.size()+1) && (line[n] == '>')){
            if((line.size() > n+2) && (line[n+2] == ' ')){
                line    = line.substr(n+2);
            } else 
                line    = line.substr(n+1);
            return true;
        }
        return false;
    }
    
    void    Markdown::quote_apply(std::string_view sv)
    {
        m_stream << "<blockquote>\n";
        quote_line(sv);
        
    }
    
    bool    Markdown::quote_line(std::string_view txt)
    {
        
    
        return false;
    }
    
    void    Markdown::quote_finish()
    {
        m_stream << "</blockquote>\n";
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const Markdown::Block  Markdown::s_blocks[] = {
        { &Markdown::code_block_match, &Markdown::code_block_apply, &Markdown::code_block_line, &Markdown::code_block_finish, },
        { &Markdown::header_line_match, &Markdown::header_line_apply, nullptr, nullptr },
        { &Markdown::horz_rule_match, &Markdown::horz_rule_apply, nullptr, nullptr }
    };

    void    Markdown::process(std::string_view text)
    {
        vsplit(text, '\n', [&](std::string_view line) {
            line = trimmed_end(line);
            if(m_stack.empty())
                push_block(line);
            
            if(!m_stack.empty()){
                const Block*b   = m_stack.top();
                if(b && b->line){
                    if((this->*(b->line))(line)){
                        if(b->finish)
                            (this->*(b->finish))();
                        m_stack.pop();
                    }
                }
            } 
        });
    }
    
    void    Markdown::pop_block()
    {
        if(m_stack.empty())
            return;

        const Block* blk = nullptr;
        m_stack >> blk;
        if(blk && blk->finish)
            (this->*(blk->finish))();
    }
    

    bool    Markdown::push_block(std::string_view line)
    {
        for(const Block& b : s_blocks){
            if(!b.match)
                continue;
            if(!(this->*b.match)(line))
                continue;
            if(b.apply)
                (this->*b.apply)(line);
            if(b.line){
                m_stack << &b;
                return true;
            } else
                return false;
        }
        
        return false;
    }

#if 0

        size_t is_code_line(std::string_view sv)
        {
            if(sv.empty())
                return 0;
            if(sv[0] == '\t')
                return 1;
            if(starts(sv, "    "))
                return count_starts(sv, ' ');
            return 0;
        }
        
        size_t is_break_line(std::string_view sv)
        {
            if(sv.empty())
                return 0;

            switch(sv[0]){
            case '\n':
                return 1;
            case '\r':
                if(sv.size() <= 1)
                    return 1;
                return (sv[1] == '\n')?2:1;
            default:
                return 0;
            }
        }

        size_t  is_bullet(std::string_view);
        size_t  is_checklist(std::string_view);
#endif
        
        
}


