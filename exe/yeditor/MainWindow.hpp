////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <tachyon/ui/Widget.hpp>

class MainWindow : public yq::tachyon::Widget {
    YQ_OBJECT_DECLARE(MainWindow, yq::tachyon::Widget)
public:

    MainWindow();
    ~MainWindow();
    
    void   imgui_(yq::tachyon::ViContext&u) override;
    bool    edit(const std::filesystem::path& pth);
};


