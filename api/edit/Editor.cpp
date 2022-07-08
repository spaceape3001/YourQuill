////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Editor.hpp"
#include <imgui.h>

YQ_OBJECT_IMPLEMENT(yq::edit::Editor)

namespace yq {
    namespace edit {
        EditorInfo::EditorInfo(std::string_view n, engine::WidgetInfo& base, const std::source_location& sl) : WidgetInfo(n, base, sl)
        {
        }

        Editor::Editor()
        {
        }
        
        Editor::~Editor()
        {
        }

        void    Editor::draw(engine::UiData&ui) 
        {
            if(ImGui::BeginMainMenuBar()){
                draw_menubar();
                ImGui::EndMainMenuBar();
            }
            draw_content();
        }
    }
}
