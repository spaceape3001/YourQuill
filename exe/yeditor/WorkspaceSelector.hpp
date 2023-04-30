////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <tachyon/ui/Widget.hpp>
#include "History.hpp"

struct Config;

class WorkspaceSelector : public yq::tachyon::Widget {
    YQ_OBJECT_DECLARE(WorkspaceSelector, Widget)
public:

    Config&     m_config;
    History     m_history;

    WorkspaceSelector(Config&);
    ~WorkspaceSelector();
    
    void    toMain();
    void    imgui_(yq::tachyon::ViContext&) override;
    void    vulkan_(yq::tachyon::ViContext&) override;
};

bool    init_workspace(const std::filesystem::path& pth);
bool    init_workspace(uint16_t);
