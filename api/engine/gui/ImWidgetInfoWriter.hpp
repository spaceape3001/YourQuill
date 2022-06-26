////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/gui/ImWidget.hpp>

namespace yq {
    namespace engine {
        template <typename C>
        class ImWidgetInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            Writer(ImWidgetInfo* widgetInfo) : ObjectInfo::Writer<C>(widgetInfo)
            {
            }
            
            Writer(ImWidgetInfo& widgetInfo) : Writer(&widgetInfo)
            {
            }
        };
    }
}

