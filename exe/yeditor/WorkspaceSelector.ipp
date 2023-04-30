////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WorkspaceSelector.hpp"
#include "MainWindow.hpp"
#include "Config.hpp"
#include <imgui.h>
#include <tachyon/Viewer.hpp>


WorkspaceSelector::WorkspaceSelector(Config& cfg) : m_config(cfg) 
{
    m_history.load(m_config.historyFile);
}

WorkspaceSelector::~WorkspaceSelector()
{
}


void    WorkspaceSelector::toMain()
{
    viewer() -> set_widget(new MainWindow(m_config));
}

void   WorkspaceSelector::imgui_(yq::tachyon::ViContext&u)  
{
    ImGui::ShowMetricsWindow();
    ImGui::Begin("Workspace Selection", nullptr, ImGuiWindowFlags_NoCollapse);
    
        ImGui::Text("Workspace needs to be selected");
        
        //End();
    //}
    
    if(ImGui::Button("Okay")){
        toMain();
    }
    
    ImGui::End();
}

void        WorkspaceSelector::vulkan_(yq::tachyon::ViContext& ctx) 
{
    //  a simple background will go here... 
}

YQ_OBJECT_IMPLEMENT(WorkspaceSelector)
