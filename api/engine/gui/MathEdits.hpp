////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <imgui.h>
#include <math/preamble.hpp>
#include <math/MKS.hpp>
#include <math/SCALED.hpp>
#include "StdEdits.hpp"

namespace yq {
    namespace gui {
    
        bool    DragDouble2(const char* label, Vector2D*, float speed, double v_min=0.0, double v_max=0.0, const char* format="%.3lf", ImGuiSliderFlags flags=0);
        bool    DragDouble3(const char* label, Vector3D*, float speed, double v_min=0.0, double v_max=0.0, const char* format="%.3lf", ImGuiSliderFlags flags=0);
        bool    DragDouble4(const char* label, Vector4D*, float speed, double v_min=0.0, double v_max=0.0, const char* format="%.3lf", ImGuiSliderFlags flags=0);
   
        bool    InputDouble2(const char* label, Vector2D*, const char* format = "%.3lf", ImGuiInputTextFlags flags = 0);
        bool    InputDouble3(const char* label, Vector3D*, const char* format = "%.3lf", ImGuiInputTextFlags flags = 0);
        bool    InputDouble4(const char* label, Vector4D*, const char* format = "%.3fl", ImGuiInputTextFlags flags = 0);
        bool    InputDouble4(const char* label, Quaternion3D*, const char* format = "%.3fl", ImGuiInputTextFlags flags = 0);
        
        template <typename DIM>
        bool    InputDouble(const char* label, MKS<double,DIM>* v, MKS<double,DIM> step = { 0.0 }, MKS<double,DIM> step_fast = { 0.0 }, const char* format = "%.6f", ImGuiInputTextFlags flags = 0)
        {
            return ImGui::InputDouble(label, (double*) v, step.value, step_fast.value, format, flags);
        }

        template <typename DIM, double K>
        bool    InputDouble(const char* label, SCALED<double,DIM,K>* v, SCALED<double,DIM,K> step = { 0.0 }, SCALED<double,DIM,K> step_fast = { 0.0 }, const char* format = "%.6f", ImGuiInputTextFlags flags = 0)
        {
            return ImGui::InputDouble(label, (double*) v, step.value, step_fast.value, format, flags);
        }

        template <typename DIM>
        bool    InputDouble2(const char* label, Vector2<MKS<double,DIM>>* v, const char* format = "%.6f", ImGuiInputTextFlags flags = 0)
        {
            return InputDouble2(label, (double*) v, format, flags);
        }

        template <typename DIM>
        bool    InputDouble3(const char* label, Vector3<MKS<double,DIM>>* v, const char* format = "%.6f", ImGuiInputTextFlags flags = 0)
        {
            return InputDouble3(label, (double*) v, format, flags);
        }

        template <typename DIM>
        bool    InputDouble4(const char* label, Vector4<MKS<double,DIM>>* v, const char* format = "%.6f", ImGuiInputTextFlags flags = 0)
        {
            return InputDouble4(label, (double*) v, format, flags);
        }
    }
}
