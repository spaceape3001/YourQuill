////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <math/Size2.hpp>

namespace yq {

    namespace {
        ByteArray&      byteArrayFor(WebContext& ctx)
        {
            if(!ctx.tx_content)
                ctx.tx_content  = std::make_shared<ByteArray>();
            return *(ctx.tx_content);
        }
    }

    WebHtml::WebHtml(WebContext&ctx, const std::string_view& _title) : m_context(ctx)
    {
        ctx.var_body.reserve(65536);
        if(_title.empty()){
            ctx.var_title.reserve(512);
        } else {
            ctx.var_title       = _title;
        }
    }
    
    WebHtml::~WebHtml()
    {
        run_me();
    }

    void    WebHtml::add_hscript(std::string_view s)
    {
        m_hScripts.push_back(copy(s));
    }
    
    void    WebHtml::add_hcss(std::string_view s)
    {
        m_hCss.push_back(copy(s));
    }

    void    WebHtml::run_me()
    {
        auto& ba = byteArrayFor(m_context);
        ba.reserve(8192+m_context.var_body.size());   // an estimate....
        auto temp   = web::html_template();
        m_context.tx_content_type   = ContentType::html;
        
        m_dest      = &ba;
        Target  oldT    = m_target;
        m_target    = DEST;
        temp -> execute(*this);
        m_target    = oldT;
    }

    void WebHtml::title(const std::string_view& _title)
    {
        m_context.var_title     = _title;
    }

    bool WebHtml::write(const char* buf, size_t cb) 
    {
        switch(m_target){
        case BODY:
            m_context.var_body.append(buf, cb);
            break;
        case TITLE:
            m_context.var_title.append(buf, cb);
            break;
        case SCRIPT:
            m_context.var_script.append(buf, cb);
            break;
        case DEST:
            if(m_dest)
                m_dest -> append(buf, cb);
            break;
        }
        return true;
    }
    
    void    WebHtml::_class(std::string_view clsId)
    {
        if(!clsId.empty()){
            *this << " class=\"";
            html_escape_write(*this, clsId);
            *this << "\"";
        }
    }
    
    //  ------------------------------

    WebAutoClose              WebHtml::b()
    {
        *this << "<b>"sv;
        return WebAutoClose(*this, "</b>"sv);
    }
    
    WebAutoClose              WebHtml::bold()
    {
        *this << "<b>"sv;
        return WebAutoClose(*this, "</b>"sv);
    }
    
    WebAutoClose              WebHtml::bullets()
    {
        *this << "<ul>"sv;
        return WebAutoClose(*this, "</ul>"sv);
    }

    WebAutoClose              WebHtml::h1()
    {
        *this << "<h1>"sv;
        return WebAutoClose(*this, "</h1>"sv);
    }
    
    void                WebHtml::h1(std::string_view s)
    {
        auto wt = h1();
        html_escape_write(*this, s);
    }
    
    WebAutoClose              WebHtml::h2()
    {
        *this << "<h2>"sv;
        return WebAutoClose(*this, "</h2>"sv);
    }
    
    void                WebHtml::h2(std::string_view s)
    {
        auto wt = h2();
        html_escape_write(*this, s);
    }
    
    WebAutoClose              WebHtml::h3()
    {
        *this << "<h3>"sv;
        return WebAutoClose(*this, "</h3>"sv);
    }

    void                WebHtml::h3(std::string_view s)
    {
        auto wt = h3();
        html_escape_write(*this, s);
    }
   
    WebAutoClose              WebHtml::h4()
    {
        *this << "<h4>"sv;
        return WebAutoClose(*this, "</h4>"sv);
    }

    void                WebHtml::h4(std::string_view s)
    {
        auto wt = h4();
        html_escape_write(*this, s);
    }
    
    WebAutoClose              WebHtml::h5()
    {
        *this << "<h5>"sv;
        return WebAutoClose(*this, "</h5>"sv);
    }
    
    void                WebHtml::h5(std::string_view s)
    {
        auto wt = h5();
        html_escape_write(*this, s);
    }
    
    WebAutoClose              WebHtml::h6()
    {
        *this << "<h6>"sv;
        return WebAutoClose(*this, "</h6>"sv);
    }
    
    void                WebHtml::h6(std::string_view s)
    {
        auto wt = h6();
        html_escape_write(*this, s);
    }
    
    WebAutoClose              WebHtml::i()
    {
        *this << "<i>"sv;
        return WebAutoClose(*this, "</i>"sv);
    }
    
    WebAutoClose              WebHtml::italic()
    {
        *this << "<i>"sv;
        return WebAutoClose(*this, "</i>"sv);
    }
    
    WebAutoClose        WebHtml::kvrow(std::string_view key, const UrlView& url)
    {
        *this << "<tr><th align=\"left\">";
        bool    a   = !url.empty();
        if(a)
            *this << "<a href=\"" << url << "\">";
        html_escape_write(*this, key);
        if(a)
            *this << "</a>";
        *this << "</th><td>";
        return WebAutoClose(*this, "</td></tr>\n"sv);
    }
    
    WebAutoClose              WebHtml::li()
    {
        *this << "<li>"sv;
        return WebAutoClose(*this, "</li>\n"sv);
    }
    
    WebAutoClose        WebHtml::link(const UrlView& url)
    {
        *this << "<a href=\"" << url << "\">";
        return WebAutoClose(*this, "</a>"sv);
    }
    
    WebAutoClose              WebHtml::numbers()
    {
        *this << "<ol>"sv;
        return WebAutoClose(*this, "</ol>"sv);
    }
    
    WebAutoClose              WebHtml::p()
    {
        *this << "<p>"sv;
        return WebAutoClose(*this, "</p>"sv);
    }
    
    WebAutoClose              WebHtml::paragraph()
    {
        *this << "<p>"sv;
        return WebAutoClose(*this, "</p>"sv);
    }
    
    WebAutoClose              WebHtml::pre()
    {
        *this << "<pre>"sv;
        return WebAutoClose(*this, "</pre>"sv);
    }

    void            WebHtml::pre(std::string_view s)
    {
        auto wt = pre();
        html_escape_write(*this, s);
    }

    WebAutoClose  WebHtml::script()
    {
        Target  oldTarget   = m_target;
        m_target            = SCRIPT;
        return WebAutoClose(*this, [oldTarget](WebHtml& h){
            h.m_target  = oldTarget;
        });
    }

    HtmlTable           WebHtml::table()
    {
        *this << "<table>";
        return HtmlTable(*this);
    }
    
    HtmlTable        WebHtml::table(class_t, std::string_view cls)
    {
        *this << "<table";
        _class(cls);
        *this << ">";
        return HtmlTable(*this);
    }

    WebAutoClose  WebHtml::title()
    {
        Target  oldTarget   = m_target;
        m_target            = TITLE;
        return WebAutoClose(*this, [oldTarget](WebHtml& h){
            h.m_target  = oldTarget;
        });
    }

    WebAutoClose              WebHtml::u()
    {
        *this << "<u>"sv;
        return WebAutoClose(*this, "</u>"sv);
    }
    
    WebAutoClose              WebHtml::underline()
    {
        *this << "<u>"sv;
        return WebAutoClose(*this, "</u>"sv);
    }
    
    //  -----------------------------------------------

    HtmlTable::HtmlTable()
    {
    }

    HtmlTable::HtmlTable(WebHtml& h) : WebAutoClose(h, "</table>\n"sv)
    {   
    }
    
    HtmlTable::HtmlTable(HtmlTable&&mv) : WebAutoClose(std::move(mv))
    {
    }
    
    HtmlTable& HtmlTable::operator=(HtmlTable&&mv)
    {
        WebAutoClose::operator=(std::move(mv));
        return *this;
    }
    
    HtmlTable::~HtmlTable()
    {
    }

    HtmlTable::Row    HtmlTable::row()
    {
        if(m_html){
            *m_html << "<tr>";
            return Row(*this);
        } else
            return Row();
    }

    //  -----------------------------------------------

    HtmlTable::Row::Row(HtmlTable& h) : WebAutoClose(*(h.m_html), "</tr>\n"sv)
    {
    }
    
    HtmlTable::Row::~Row()
    {
    }

    WebAutoClose    HtmlTable::Row::_cell(std::string_view clsId, const Size2U&sz)
    {
        if(m_html){
            *m_html << "<td";
            m_html -> _class(clsId);
            _size(sz);
            *m_html << ">";
            return WebAutoClose(*m_html, "</td>");
        } else
            return WebAutoClose();
    }

    WebAutoClose    HtmlTable::Row::_header(std::string_view clsId)
    {
        if(m_html){
            *m_html << "<th";
            m_html -> _class(clsId);
            *m_html << ">";
            return WebAutoClose(*m_html, "</th>");
        } else
            return WebAutoClose();
    }
    
    
    void    HtmlTable::Row::_size(const Size2U& sz)
    {
        if(sz.x > 1)
            *m_html << " colspan=\"" << sz.x << "\"";
        if(sz.y > 1)
            *m_html << " rowspan=\"" << sz.x << "\"";
    }
    
    WebAutoClose    HtmlTable::Row::cell()
    {
        return _cell("", ZERO);
    }
    
    WebAutoClose    HtmlTable::Row::cell(const Size2U&sz)
    {
        return _cell("", sz);
    }

    WebAutoClose    HtmlTable::Row::cell(class_t, std::string_view clsId)
    {
        return _cell(clsId, ZERO);
    }

    WebAutoClose    HtmlTable::Row::cell(class_t, std::string_view clsId, const Size2U& sz)
    {
        return _cell(clsId, sz);
    }

    HtmlTable::Row&   HtmlTable::Row::cell(std::string_view txt)
    {
        if(m_html){
            auto h = cell();
            html_escape_write(*m_html, txt);
        }
        return *this;
    }


    WebAutoClose    HtmlTable::Row::header()
    {
        return _header("");
    }
    
    
    WebAutoClose    HtmlTable::Row::header(class_t, std::string_view clsId)
    {
        return _header(clsId);
    }
    

    HtmlTable::Row&   HtmlTable::Row::header(std::string_view txt)
    {
        if(m_html){
            auto h = header();
            html_escape_write(*m_html, txt);
        }
        return *this;
    }
    
    

}
