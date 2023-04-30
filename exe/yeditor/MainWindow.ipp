////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MainWindow.hpp"
#include <imgui.h>

MainWindow::MainWindow() 
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

