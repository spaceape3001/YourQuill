////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ImWidget.hpp"
#include "ImWidgetInfoWriter.hpp"

YQ_OBJECT_IMPLEMENT(yq::engine::ImWidget)

namespace yq {
    namespace engine {
        ImWidgetInfo::ImWidgetInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(zName, base, sl)
        {
            set_option(WIDGET);
        }

        ImWidget::ImWidget()
        {
        }
        
        ImWidget::~ImWidget()
        {
            for(ImWidget *w  : m_children)
                delete w;
            m_children.clear();
        }
    }
}
