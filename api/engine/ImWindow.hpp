////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/Viewer.hpp>
#include <imgui.h>

namespace yq {
    namespace engine {

        /*! \brief Base window class for pure ImGUI windows
        */
        class ImWindow : public Viewer {
            YQ_OBJECT_DECLARE(ImWindow, Viewer)
        public:
            ImWindow(const ViewerCreateInfo& wci = ViewerCreateInfo());
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
