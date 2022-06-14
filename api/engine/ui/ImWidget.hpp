////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <basic/Object.hpp>
#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>

namespace yq {
    namespace engine {
        class ImWidgetInfo : public ObjectInfo {
        public:
            template <typename C> class Writer;
        
            ImWidgetInfo(std::string_view, const ObjectInfo&, const std::source_location& sl = std::source_location::current());
        };
        
        /*! \brief Root something that's drawwable on ImGui
        
            By default, it autosizes itself per ImGui's rules.  
            (We'll adjust later if necessary)
        */
        class ImWidget : public Object, trait::not_copyable, trait::not_moveable {    
            YQ_OBJECT_INFO(ImWidgetInfo)
            YQ_OBJECT_DECLARE(ImWidget, Object)
        public:
        
        
            virtual void    draw_imgui(){}
            
        protected:
            ImWidget();
            ~ImWidget();
            
            ImWidget*               m_parent    = nullptr;
            std::vector<ImWidget*>  m_children;
        };
    }
}
