////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "StdEdits.hpp"
#include "MathEdits.hpp"
#include <math/Quaternion3.hpp>
#include <math/Vector2.hpp>
#include <math/Vector3.hpp>
#include <math/Vector4.hpp>

namespace yq {
    namespace gui {
        bool Checkbox(const char*z, bool&v)
        {
            return ImGui::Checkbox(z, &v);
        }

        bool    DragDouble(const char* label, double*v, float v_speed, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
        {
            return ImGui::DragScalar(label, ImGuiDataType_Double, v, v_speed, &v_min, &v_max, format, flags);
        }
        
        bool    DragDouble(const char* label, double&v, float v_speed, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
        {
            return ImGui::DragScalar(label, ImGuiDataType_Double, &v, v_speed, &v_min, &v_max, format, flags);
        }
        
        bool    DragDouble2(const char* label, double v[2], float v_speed, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
        {
            return ImGui::DragScalarN(label, ImGuiDataType_Double, v, 2, v_speed, &v_min, &v_max, format, flags);
        }

        bool    DragDouble2(const char* label, Vector2D*v, float speed, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
        {
            return DragDouble2(label, (double*) v, speed, v_min, v_max, format, flags);
        }
        
        bool    DragDouble3(const char* label, double v[3], float v_speed, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
        {
            return ImGui::DragScalarN(label, ImGuiDataType_Double, v, 3, v_speed, &v_min, &v_max, format, flags);
        }
        
        bool    DragDouble3(const char* label, Vector3D*v, float speed, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
        {
            return DragDouble3(label, (double*) v, speed, v_min, v_max, format, flags);
        }
        
        bool    DragDouble4(const char* label, double v[4], float v_speed, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
        {
            return ImGui::DragScalarN(label, ImGuiDataType_Double, v, 4, v_speed, &v_min, &v_max, format, flags);
        }
        
        bool    DragDouble4(const char* label, Vector4D*v, float speed, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
        {
            return DragDouble4(label, (double*) v, speed, v_min, v_max, format, flags);
        }
        
        bool    InputDouble2(const char* label, double v[2], const char* format, ImGuiInputTextFlags flags)
        {
            return ImGui::InputScalarN(label, ImGuiDataType_Double, v, 2, NULL, NULL, format, flags);
        }
        
        bool    InputDouble2(const char* label, Vector2D*v, const char* format, ImGuiInputTextFlags flags)
        {
            return ImGui::InputScalarN(label, ImGuiDataType_Double, v, 2, NULL, NULL, format, flags);
        }

        bool    InputDouble3(const char* label, double v[3], const char* format, ImGuiInputTextFlags flags)
        {
            return ImGui::InputScalarN(label, ImGuiDataType_Double, v, 3, NULL, NULL, format, flags);
        }
        
        bool    InputDouble3(const char* label, Vector3D*v, const char* format, ImGuiInputTextFlags flags)
        {
            return ImGui::InputScalarN(label, ImGuiDataType_Double, v, 3, NULL, NULL, format, flags);
        }

        bool    InputDouble4(const char* label, double v[4], const char* format, ImGuiInputTextFlags flags)
        {
            return ImGui::InputScalarN(label, ImGuiDataType_Double, v, 4, NULL, NULL, format, flags);
        }

        bool    InputDouble4(const char* label, Vector4D*v, const char* format, ImGuiInputTextFlags flags)
        {
            return ImGui::InputScalarN(label, ImGuiDataType_Double, v, 4, NULL, NULL, format, flags);
        }

        bool    InputDouble4(const char* label, Quaternion3D*v, const char* format, ImGuiInputTextFlags flags)
        {
            return ImGui::InputScalarN(label, ImGuiDataType_Double, v, 4, NULL, NULL, format, flags);
        }

        // Taken from https://github.com/ocornut/imgui/issues/1537
        void ToggleButton(const char* str_id, bool* v)
        {
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            float height = ImGui::GetFrameHeight();
            float width = height * 1.55f;
            float radius = height * 0.50f;

            if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
                *v = !*v;
            ImU32 col_bg;
            if (ImGui::IsItemHovered())
                col_bg = *v ? IM_COL32(145+20, 211, 68+20, 255) : IM_COL32(218-20, 218-20, 218-20, 255);
            else
                col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);

            draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
            draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
        }

        bool    VSliderDouble(const char* label, const ImVec2& size, double* v, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
        {
            return ImGui::VSliderScalar(label, size, ImGuiDataType_Double, v, &v_min, &v_max, format, flags);
        }
        
        bool    VSliderDouble(const char* label, const ImVec2& size, double& v, double v_min, double v_max, const char* format, ImGuiSliderFlags flags)
        {
            return ImGui::VSliderScalar(label, size, ImGuiDataType_Double, &v, &v_min, &v_max, format, flags);
        }
    }
}
