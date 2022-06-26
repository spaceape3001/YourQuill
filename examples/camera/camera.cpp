////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    This example will be about combinging ImGUI and the vulkan to have some controls
    as we play around with the camera.
*/

#include <asset/Colors.hpp>
#include <asset/NullCamera.hpp>
#include <asset/SpaceCamera.hpp>
#include <asset/Triangle.hpp>

#include <basic/DelayInit.hpp>
#include <engine/Application.hpp>
#include <engine/Scene.hpp>
#include <engine/Perspective.hpp>
#include <engine/gui/ImWindow.hpp>
#include <engine/gui/MathEdits.hpp>

#include <math/RGB.hpp>
#include <math/Vector2.hpp>
#include <math/shape_math.hpp>
#include <math/vector_math.hpp>
#include <math/Units.hpp>
#include <math/trig.hpp>
#include <math/Triangle2.hpp>
#include <math/Triangle3.hpp>
#include <imgui.h>
#include <chrono>

using namespace ImGui;
using namespace yq;
using namespace yq::gui;
using namespace yq::asset;
using namespace yq::engine;

const auto  TriPoints   = Triangle2D{{1.0,1.0}, {0., -1},{-1., 1.}};
const auto  TriColors   = TriangleData<RGB3F>{ (RGB3F) color::Red, (RGB3F) color::White, (RGB3F) color::Blue };

using timepoint_t   = std::chrono::time_point<std::chrono::steady_clock>;

struct CameraWin : public ImWindow {
    YQ_OBJECT_DECLARE(CameraWin, engine::ImWindow)

    timepoint_t             start;
    Ref<SpaceCamera>        cam;
    Scene                   scene;
    Perspective             view;
    bool                    show_camera = true;
    bool                    slave_clock = true;

    CameraWin(const WindowCreateInfo& wci) : ImWindow(wci)
    {
        start   = std::chrono::steady_clock::now();
        cam         = new SpaceCamera;
        cam->set_orientation(hpr((Radian) 0._deg, (Radian) -90._deg, (Radian) 0._deg));
        cam->set_near(.01);
        cam->set_far(200.);
        view.camera = cam.ptr(); // new NullCamera; 
        
        Ref<Triangle>   tri = new Triangle(xy(TriPoints, 0.0), rgba(TriColors, 0.5));
        tri->set_scaling(0.5);
        scene.things.push_back(tri);
    }

    void        draw_vulqan(VkCommandBuffer cmdbuf) override
    {
        timepoint_t n   = std::chrono::steady_clock::now();
        
        if(slave_clock){
            std::chrono::duration<double>  diff    = start - n;
        
            Degree      angle{ diff.count() };
            
            auto ca     = cos((Radian) angle);
            auto sa     = sin((Radian) angle);
            Vector3D    p2{ 10. * ca, 0., 10 * sa };
            
            cam->set_position(p2);
            cam->set_orientation(rotor_y((Radian) -angle));
        }

        render(cmdbuf, scene, view);
        ImWindow::draw_vulqan(cmdbuf);
    }
    
    void    draw_imgui() override
    {
        if(Begin("Camera", &show_camera)){
            gui::ToggleButton("motion", &slave_clock);
        
            Vector3D        pos = cam->position();
            if(InputDouble3("position", &pos) && !slave_clock)
                cam->set_position(pos);
            Quaternion3D    orient  = cam->orientation();
            if(InputDouble4("orientation", &orient) && !slave_clock)
                cam->set_orientation( ~orient);
        
            double          n   = cam->near();
            if(InputDouble("Near", &n))
                cam->set_near(n);
            double          f   = cam->far();
            if(InputDouble("Far", &f))
                cam->set_far(f);
            Degree          fov = cam->fov();
            if(InputDouble("FOV", &fov, 0.1_deg))
                cam->set_fov(fov);
        
            Vector3D    scale   = cam->scale();
            if(InputDouble3("Scale", &scale))
                cam->set_scale(scale);

            End();
        }
    }
    
};

YQ_OBJECT_IMPLEMENT(CameraWin)

int main(int argc, char* argv[])
{
    AppCreateInfo        vi;
    
    Application app(argc, argv, vi);
    //load_plugin_dir("plugin");
    app.finalize();
    
    WindowCreateInfo      wi;
    wi.title        = "Cameras!";
    wi.resizable    = true;
    wi.size         = { 1920, 1080 };
    wi.clear        = { 0.1f, 0.1f, 0.2f, 1.f };

    Ref<CameraWin>   win = new CameraWin(wi);
    if(!win->good())
        return -1;
    
    app.run_window(win.ptr());
    return 0;
}



