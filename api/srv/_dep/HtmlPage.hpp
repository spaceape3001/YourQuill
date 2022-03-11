////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "HtmlWriter.hpp"
#include "Page.hpp"

namespace html {
    class TabBar {
    public:
        TabBar(std::initializer_list<Page::Writer>);
        const Vector<const Page*>&      pages() const { return m_pages; }
    private:
        Vector<const Page*>             m_pages;
    };
}

Page::Writer        reg_page(HttpOp, const String&, void (*)(HtmlWriter&));
Page::Writer        reg_dispatcher(HttpOp, const String&, void(*)(HtmlWriter&, const String&));
void                reg_tabbar(std::initializer_list<Page::Writer>);
