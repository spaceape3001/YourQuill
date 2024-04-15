////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq::mithril {
    Template::Template(ContentType ct) : m_type(ct)
    {
        parse(szDefaultTemplate);
    }

    Template::Template(std::string&& mv, ContentType ct) : m_data(std::move(mv)), m_type(ct)
    {
        parse(m_data);
    }
    
    Template::Template(std::string_view k, ContentType ct) : m_data(k), m_type(ct)
    {
        parse(m_data);
    }

    void    Template::execute(WebHtml&h) const
    {
        //  TODO .... content type == markdown
    
        for(auto& t : m_bits){
            if(!t.variable){
                h << t.token;
                continue;
            }
            
            const WebVariable*  v = web::variable(t.token);
            if(v){
                v -> handle(h);
            } else {
                h << "{{" << t.token << "}}"; // make it clear there's a substitution failure
            }
        }
    }
    

    void    Template::parse(std::string_view data)
    {
        size_t n   = data.find("{{");
        size_t m   = 0;
        
        if(n == std::string_view::npos){        // no variables... gotcha
            m_bits.push_back(Token{ data, false });
            return;
        }
        
        while(n < data.size()){
            if(n > m){
                //  There's some content before "{{", capture it
                m_bits.push_back(Token{ data.substr(m, n-m), false });
            }

            n += 2;
            m   = data.find("}}", n);
            if(m>=data.size()){ // badly truncated, dropping....
                n   = data.size();
                break;
            }

            std::string_view     k = data.substr(n, m-n);
            m_bits.push_back(Token{ k, true });
            m_vars.insert(k);
            
            if(m>=data.size())
                break;
            
            m += 2;
            n   = data.find("{{", m);
        }
        
        if(m < data.size()){
                // no more, so push the remainder on
            m_bits.push_back(Token{ data.substr(m), false });
        }

            // YES, possible to lose between "{{NAME" and end if there's no "}}", but that's ill-formed
        
    }

    void        Template::writeBitsToInfo() const
    {
        auto write  = [&](log4cpp::CategoryStream&& log)
        {
            log << "Template Report:\n"
                << "Variables Found: " << join(m_vars, ", ") << "\n";
                
            size_t n=1;
            for(auto& b : m_bits){
                if(b.variable){
                    log << "[" << n << "] Variable: " << b.token << "\n";
                } else {
                    log << "[" << n << "] Text : " << b.token << "\n";
                }
                ++n;
            }
        };
        write(yInfo());
    }
}
