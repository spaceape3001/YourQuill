////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Config.hpp"
#include "MainWindow.hpp"
#include <imgui.h>


struct Config;

MainWindow::MainWindow(Config& cfg) : m_config(cfg)
{
}

MainWindow::~MainWindow()
{
}

void   MainWindow::imgui_(yq::tachyon::ViContext&u) 
{
    if(ImGui::BeginMainMenuBar()){
        
    
        ImGui::EndMainMenuBar();
    }

    ImGui::ShowDemoWindow();
}


bool    MainWindow::edit(const std::filesystem::path& pth)
{
    return false;
}

YQ_OBJECT_IMPLEMENT(MainWindow)

