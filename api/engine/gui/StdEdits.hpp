////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <imgui.h>

namespace yq {
    namespace gui {
        using namespace ImGui;
        
        
        bool    Checkbox(const char*, bool&);
        bool    DragDouble(const char* label, double*, float speed, double v_min=0.0, double v_max=0.0, const char* format="%.3lf", ImGuiSliderFlags flags=0);
        bool    DragDouble(const char* label, double&, float speed, double v_min=0.0, double v_max=0.0, const char* format="%.3lf", ImGuiSliderFlags flags=0);
        bool    DragDouble2(const char* label, double v[2], float speed, double v_min=0.0, double v_max=0.0, const char* format="%.3lf", ImGuiSliderFlags flags=0);
        bool    DragDouble3(const char* label, double v[3], float speed, double v_min=0.0, double v_max=0.0, const char* format="%.3lf", ImGuiSliderFlags flags=0);
        bool    DragDouble4(const char* label, double v[4], float speed, double v_min=0.0, double v_max=0.0, const char* format="%.3lf", ImGuiSliderFlags flags=0);

        bool    InputDouble2(const char* label, double v[2], const char* format = "%.3lf", ImGuiInputTextFlags flags = 0);
        bool    InputDouble3(const char* label, double v[3], const char* format = "%.3lf", ImGuiInputTextFlags flags = 0);
        bool    InputDouble4(const char* label, double v[4], const char* format = "%.3fl", ImGuiInputTextFlags flags = 0);

        //! Renders a toggle-slider switch
        //! From https://github.com/ocornut/imgui/issues/1537
        void    ToggleButton(const char* str_id, bool* v);

        bool    VSliderDouble(const char* label, const ImVec2& size, double* v, double v_min, double v_max, const char* format = "%.l3f", ImGuiSliderFlags flags = 0);
        bool    VSliderDouble(const char* label, const ImVec2& size, double& v, double v_min, double v_max, const char* format = "%.l3f", ImGuiSliderFlags flags = 0);

   }
}
