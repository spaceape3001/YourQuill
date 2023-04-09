////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/HttpParser.hpp>
#include "JsonAdapter.hpp"
#include "TypedBytes.hpp"
#include "PageTemplate.hpp"
#include "VarTemplate.hpp"
#include "WebAdapters.hpp"
#include "WebContext.hpp"
#include "WebHtml.hpp"
#include "WebImage.hpp"
#include "WebPage.hpp"
#include "WebRedirect.hpp"
#include "WebTemplate.hpp"
#include "WebVariable.hpp"
//#include "NetWriter.hpp"

#include <basic/Random.hpp>
#include <basic/Safety.hpp>
#include <basic/EnumMap.hpp>
#include <basic/Map.hpp>
#include <io/FileUtils.hpp>
#include <basic/Logging.hpp>
#include <io/stream/Bytes.hpp>
#include <io/StreamOps.hpp>
#include <io/stream/Text.hpp>
#include <basic/Encode64.hpp>
#include <basic/TextUtils.hpp>

#include <mithril/kernel/notify/FileNotifyAdapters.hpp>
#include <mithril/kernel/notify/Stage4.hpp>


#include <asio/write.hpp>
#include <nlohmann/json.hpp>
#include <tbb/spin_rw_mutex.h>
#include <unistd.h>

#include "WebRepo.ipp"

namespace yq {
    namespace {
        const Set<std::string_view, IgCase>&     httpEntities()
        {
            static const Set<std::string_view, IgCase>    ret({
                "&amp;",
                "&lt;",
                "&gt;"
            });
            return ret;
        }
        
        size_t  is_entity(std::string_view text)
        {
            if(text.empty())
                return 0;
            if(text[0] != '&')
                return 0;
            
            const char* sc  = strnchr(text, ';');
            if(!sc)
                return 0;

            const char* amp = strnchr(text.data()+1, text.size()-1, '&');
            if(amp && (amp < sc))
                return 0;

                // size of this snippet
            size_t  sz  = sc-text.data()+1;
            if(text[1] == '#'){
                for(const char* y=text.data()+2; (y<sc); ++y)
                    if(!is_digit(*y))
                        return 0;
                return sz;
            }
            
            static auto & entities = httpEntities();
            if( entities.has(text.substr(0, sz)))
                return sz;
            return 0;
        }
        
        
        
        template <typename Pred>
        void        iter_http(std::string_view text, Pred p)
        {
            if(text.empty())
                return ;
            
            const char*     next    = nullptr;
            for(const char& c : text){
                if(next && (&c<next)){
                    p(c);
                } else {
                    size_t  sc  = is_entity(text.substr(&c-text.data()));
                    if(sc){
                        next    = &c + sc;
                        p(c);
                    } else {
                        switch(c){
                        case '&':
                            p('&');
                            p('a');
                            p('m');
                            p('p');
                            p(';');
                            break;
                        case '<':
                            p('&');
                            p('l');
                            p('t');
                            p(';');
                            break;
                        case '>':
                            p('&');
                            p('g');
                            p('t');
                            p(';');
                            break;
                        case '"':
                            p('&');
                            p('q');
                            p('u');
                            p('o');
                            p('t');
                            p(';');
                            break;
                        case '\'':
                            p('&');
                            p('a');
                            p('p');
                            p('o');
                            p('s');
                            p(';');
                            break;
                        default:
                            p(c);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    std::string     html_escape(std::string_view sv)
    {
        static constexpr const std::string_view     kReplacementChar    = "\xEF\xBF\xBD"sv;
        std::string ret;
        ret.reserve(sv.size());
        iter_http(sv, [&](char c){
            if(!c) [[unlikely]] {
                ret += kReplacementChar;
            } else
                ret += c;
        });
        return ret;
    }

    void            html_escape_write(Stream& s, std::string_view sv)
    {
        static constexpr const std::string_view     kReplacementChar    = "\xEF\xBF\xBD"sv;
        iter_http(sv, [&](char ch){
            if(!ch) [[unlikely]]
                s << kReplacementChar;
            s << ch;
        });
    }

    void            html_escape_write(Stream& s, const std::vector<char>&sv)
    {
        return html_escape_write(s, std::string_view(sv.data(), sv.size()));
    }
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    bool    send_file_info(const std::filesystem::path& file, WebContext& rs, ContentType ct)
    {
        Ref<TypedBytes> tb  = new TypedBytes(file);
        if(!tb -> do_info()){
            rs.status = HttpStatus::NotFound;
            return false;
        }
        
        if(ct == ContentType())
            ct  = tb -> type;
            
        rs.status = HttpStatus::Success;
        rs.tx_header("Date", tb -> modified);
        rs.tx_content_type = ct;
        //rs.header("Content-Length", to_string_view(tb->size));
        return true;
    }

    bool    send_file(const std::filesystem::path& file, WebContext& rs, ContentType ct)
    {
        Ref<TypedBytes> tb  = new TypedBytes(file);
        if(!tb -> do_info()){
            rs.status = HttpStatus::NotFound;
            return false;
        }
        
        if(!tb -> do_read()){
            rs.status = tb -> status;
            return false;
        }

        if(ct == ContentType())
            ct  = tb -> type;
        
        rs.tx_content_type  = ct;
        rs.tx_content       = std::move(tb->data);
        rs.status = HttpStatus::Success;
        rs.tx_header("Date", tb -> modified);
        return true;
    }
    
    //void    append(NetWriter&, const std::filesystem::path&)
    //{
    //}

    std::string_view    web_title(std::string_view text)
    {
        if(!starts(text, "#!"))
            return std::string_view();
        size_t  n = text.find_first_of('\n');
        if(n == std::string_view::npos)
            return text;
        return std::string_view(text.data()+2, text.data()+n+1);
    }


}

#include "PageTemplate.ipp"
#include "Template.ipp"
#include "TypedBytes.ipp"
#include "VarTemplate.ipp"
#include "WebAdapters.ipp"
#include "WebAutoClose.ipp"
#include "WebContext.ipp"
#include "WebHtml.ipp"
#include "WebImage.ipp"
#include "WebPage.ipp"
#include "WebRedirect.ipp"
#include "WebTemplate.ipp"
#include "WebVariable.ipp"
