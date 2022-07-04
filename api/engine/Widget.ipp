////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Widget.hpp"

YQ_OBJECT_IMPLEMENT(yq::engine::Widget)

namespace yq {
    namespace engine {
        WidgetInfo::WidgetInfo(std::string_view zName, const MetaObjectInfo& base, const std::source_location& sl) :
            MetaObjectInfo(zName, base, sl)
        {
            set_option(WIDGET);
        }

        Widget::Widget()
        {
        }
        
        Widget::~Widget()
        {
            m_children.clear();
        }

        void    Widget::draw(UIData&  ui)
        {
            for(auto& w : m_children)
                w->draw(ui);
        }
    }
}