////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/enum/SizeDesc.hpp>
#include <yq/enum/Submit.hpp>
#include <yq/enum/Reset.hpp>
#include <optional>

namespace yq {
    class WebHtml;
    struct Root;
    
    namespace html {
    
        template <typename T>
        struct Config {
            T       data;
        };
        
        template <typename T>
        Config<T>   config(T v) 
        {
            return Config<T>{v};
        };
    
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        template <typename T>
        struct Dev {
            T       data;
        };

        template <typename T>
        Dev<T> dev(T d) 
        { 
            return Dev<T>{ d }; 
        }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        template <typename T>
        struct DevID {
            T       data;
        };

        template <typename T>
        DevID<T> dev_id(T d) 
        { 
            return DevID<T>{ d }; 
        }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        template <typename T>
        struct Edit {
            T           value;
            const Root* root = nullptr;
            bool        force_inspect = false;
        };
        
        template <typename T>
        Edit<T> edit(T v, const Root* rt=nullptr, bool force=false)
        {
            return Edit<T>{v, rt, force};
        }
        
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        struct Escape {
            std::string    text;
        };
        
        inline Escape escape(std::string_view v)
        {
            return Escape{std::string(v)};
        }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        
        template <typename T>
        struct Full {
            T       data;
        };
        
        template <typename T>
        Full<T>     full(T v) 
        {
            return Full<T>{v};
        }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        struct ICheck {
            std::string     key, label;
            bool            checked     = false;
        };
        
        inline ICheck      icheck(std::string_view k, std::string_view l, bool c)
        {
            return ICheck{std::string(k),std::string(l),c};
        }
        
        WebHtml&    operator<<(WebHtml&, const ICheck&);

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        struct IEditReq {};
        
        inline IEditReq iedit() { return IEditReq{}; }
        
        WebHtml&    operator<<(WebHtml&, IEditReq);

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        struct IKey {};
        
        WebHtml&    operator<<(WebHtml&, IKey);
        
        inline IKey ikey() { return IKey{}; }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        struct IHidden {
            std::string key, value;
        };

        WebHtml&    operator<<(WebHtml&, const IHidden&);

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        struct ILabel {
            std::string key, label;
        };
        
        inline ILabel   ilabel(std::string_view k, std::string_view l)
        {
            return ILabel(std::string(k),std::string(l));
        }
        
        WebHtml&    operator<<(WebHtml&, const ILabel&);

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        struct ILine {
            std::string     key, value;
            unsigned int    size    = 0;
        };
        
        inline ILine    iline(std::string_view k, std::string_view v, unsigned int sz=0)
        {
            return ILine(std::string(k),std::string(v),sz);
        }
        
        WebHtml&    operator<<(WebHtml&, const ILine&);
       
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
       
        struct ISubmit {
            Submit  submit;
            Reset   reset;
        };
        
        inline ISubmit  isubmit(Submit sub=Submit(), Reset res=Reset())
        {
            return ISubmit{sub,res};
        }
        
        WebHtml&    operator<<(WebHtml&, const ISubmit&);
        
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        struct IText {
            std::string     key, value;
            unsigned int    rows    = 10;
            unsigned int    cols    = 80;
        };
       
        inline IText     itext(std::string_view k, std::string_view v, unsigned int r=10, unsigned int c=10)
        {
            return IText{std::string(k),std::string(v),r,c};
        }
       
        inline IText     itext(std::string_view k, unsigned int r=10, unsigned int c=10)
        {
            return IText{std::string(k),std::string(),r,c};
        }
        
        WebHtml&    operator<<(WebHtml&, const IText&);
       
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        template <typename T>
        struct Icon {
            T               src;
            std::string     style;
            std::string     alt;
        };
        
        template <typename T>
        Icon<T> icon(T src, std::string_view sty=std::string_view(), std::string_view alt=std::string_view())
        {
            return Icon<T>{src, std::string(sty), std::string(alt)};
        }
        
        inline Icon<std::string> icon(std::string_view src, std::string_view sty=std::string_view(), std::string_view alt=std::string_view())
        {
            return Icon<std::string>{std::string(src), std::string(sty), std::string(alt)};
        }
        
        WebHtml&    operator<<(WebHtml&, const Icon<std::string>&);
       

       //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        
        struct MarkdownText {
            std::string_view    data;
        };
        
        inline MarkdownText    markdown(std::string_view k) 
        { 
            return MarkdownText{k}; 
        }
        
        WebHtml&    operator<<(WebHtml&, const MarkdownText&);

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        
        template <typename T>
        struct Plural {
            T       data;
        };
        
        template <typename T>
        Plural<T> plural(T d)
        {
            return Plural<T>{ d };
        }
        

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        WebHtml&    operator<<(WebHtml&, Reset);    
        WebHtml&    operator<<(WebHtml&, Submit);    
    }
}

