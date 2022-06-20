////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/MainWindow.hpp>
#include <imgui.h>

namespace yq {
    namespace engine {

        /*! \brief Base window class for pure ImGUI windows
        */
        class ImWindow : public MainWindow {
            YQ_OBJECT_DECLARE(ImWindow, MainWindow)
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
            
            void    upload_fonts();
        };
    }

}
