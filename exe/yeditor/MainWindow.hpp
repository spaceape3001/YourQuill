////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <tachyon/ui/Widget.hpp>

struct Config;

class MainWindow : public yq::tachyon::Widget {
    YQ_OBJECT_DECLARE(MainWindow, yq::tachyon::Widget)
public:

    Config&                         m_config;

    MainWindow(Config& cfg);
    ~MainWindow();
    
    void   imgui_(yq::tachyon::ViContext&u) override;
    bool    edit(const std::filesystem::path& pth);
};


