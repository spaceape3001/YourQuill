////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/vulqan/VqWindow.hpp>
#include <imgui.h>

namespace yq {

    /*! \brief Base window class for pure ImGUI windows
    */
    class ImWindow : public VqWindow {
        YQ_OBJECT_DECLARE(ImWindow, VqWindow)
    public:
        ImWindow(const WindowCreateInfo& wci = WindowCreateInfo());
        ~ImWindow();

        virtual bool        draw() override;
        
    protected:
        //  override this
        virtual void    draw_imgui(){}
        
        virtual void    draw_vulqan(VkCommandBuffer) override;
        
    private:
        ImGuiContext*       m_imgui = nullptr;
        VkDescriptorPool    m_descriptors   = nullptr;
        
        void    make_descriptors(uint32_t cnt=1000);
        void    upload_fonts();
    };
}

