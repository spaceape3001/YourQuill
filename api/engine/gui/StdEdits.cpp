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
