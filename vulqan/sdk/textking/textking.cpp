////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    Simple "demo" utilitiy
*/

#include <basic/Logging.hpp>
#include <basic/PluginLoader.hpp>
#include <basic/meta/Meta.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/Application.hpp>
#include <engine/Viewer.hpp>
#include <engine/Widget.hpp>
#include <engine/vulqan/VqUtils.hpp>
#include <MyImGui.hpp>
#include <iostream>
#include <TextEditor.h>
#include <ImGuiFileDialog.h>
#include <basic/FileUtils.hpp>
#include <basic/TextUtils.hpp>
#include <widget/TextEdit.hpp>

using namespace yq;
using namespace yq::engine;

class GTEditor : public engine::Widget, public TextEditor {
    YQ_OBJECT_DECLARE(GTEditor, engine::Widget)
public:
    GTEditor(const std::filesystem::path& fpath=std::filesystem::path())
    {
        if(!fpath.empty()){
            SetText(file_string(fpath));
            m_path  = fpath;
        }
        
        static uint64_t     s_id  = 1;
        
        m_textid        = std::string("TextKing:") + to_string(s_id++);
        
    }
    
    void    draw() override;
    
private:
    std::filesystem::path   m_path;
    std::string             m_textid;
};

class N2Editor : public engine::Widget {
    YQ_OBJECT_DECLARE(N2Editor, engine::Widget)
public:

    N2Editor(const std::filesystem::path& fpath=std::filesystem::path())
    {
        m_edit  = new widget::TextEdit;
    }
    
    void    draw() override;
    
    Ref<widget::TextEdit>       m_edit;
};

void GTEditor::draw()
{
    using namespace ImGui;
    auto cpos   = GetCursorPosition();
    Begin(m_textid.c_str(), nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, GetTotalLines(),
			IsOverwrite() ? "Ovr" : "Ins",
			CanUndo() ? "*" : " ",
			GetLanguageDefinition().mName.c_str(), m_path.c_str());
    TextEditor::Render("TextEditor");
    End();
}

void    N2Editor::draw() 
{
    using namespace ImGui;
    //auto cpos   = GetCursorPosition();
    PushID((int) id());
    Begin("N2Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    //Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, GetTotalLines(),
			//IsOverwrite() ? "Ovr" : "Ins",
			//CanUndo() ? "*" : " ",
			//GetLanguageDefinition().mName.c_str(), m_path.c_str());
    m_edit -> draw();
    End();
    PopID();
}

class TextKing : public Viewer {
    YQ_OBJECT_DECLARE(TextKing, Viewer)
public:
    TextKing(const ViewerCreateInfo & wci=ViewerCreateInfo ()) : Viewer(wci)
    {
    }
    
    ~TextKing()
    {
    }
    
    void   draw_imgui() override 
    {
        enum FileMode   {
            NONE    = 0,
            OPEN,
            SAVE
        };
        
        static auto*    file_dialog = IGFD::FileDialog::Instance();
        
        FileMode        file_mode   = NONE;
        const char*     file_title  = nullptr;
        
        using namespace ImGui;
        if(BeginMainMenuBar()){
            if(BeginMenu("File")){
                if(MenuItem("New")){
                    install(new GTEditor);
                }
                if(MenuItem("NewB")){
                    install(new N2Editor);
                }
                if(MenuItem("Open")){
                    //file_mode       = OPEN;
                    //file_title      = "Open Text File";
                    //file_dialog.Open(file_title, "Open a file");
                }
                if(MenuItem("Save")){
                
                }
                if(MenuItem("Save As")){
                
                }
                Separator();
                if(MenuItem("Quit")){
                    
                }
                EndMenu();
            }
            if(BeginMenu("Edit")){
                if(MenuItem("Undo")){
                } 
                if(MenuItem("Redo")){
                }
                Separator();
                if(MenuItem("Cut")){
                }
                if(MenuItem("Copy")){
                }
                if(MenuItem("Paste")){
                }
                if(MenuItem("Clear")){
                }
                Separator();
                if(MenuItem("Select All")){
                }
            }
            ImGui::EndMainMenuBar();
        }
        
        for(auto& e : m_editors)
            e->draw();
            
        switch(file_mode){
        case OPEN:
            //if(file_dialog.IsDone(file_title)){
                //if(file_dialog.HasResult()){
                    //std::string res = file_dialog.GetResult().u8string();
                    //if(!res.empty()){
                        //install(new Editor(res));
                    //}
                //}
            //}
            break;
        case SAVE:
            break;
        default:
            break;
        }
    }
    
    void                        install(GTEditor* editor)
    {
        m_editors.push_back(editor);
        m_active                = editor;
    }

    void                        install(N2Editor* editor)
    {
        m_editors.push_back(editor);
    }
    
    std::vector<Ref<engine::Widget>>    m_editors;
    GTEditor*                           m_active = nullptr;
};

YQ_OBJECT_IMPLEMENT(TextKing)
YQ_OBJECT_IMPLEMENT(GTEditor)
YQ_OBJECT_IMPLEMENT(N2Editor)

int main(int argc, char* argv[])
{
    AppCreateInfo        vi;
    vi.app_name     = "im_demo";

    Application app(argc, argv, vi);
    load_plugin_dir("plugin");
    app.finalize();
    
    ViewerCreateInfo      wi;
    wi.title        = "TextKING!";
    wi.clear        = { 0.0, 0.2, 0.5, 1. };
    wi.resizable    = true;
    wi.imgui        = true;
    //wi.pmode        = VK_PRESENT_MODE_IMMEDIATE_KHR;  // <-< Set this if you want to see how fast your CPU & GPU can go!  (Metrics under Tools menu.)
    Ref<TextKing>   window  = new TextKing(wi);
    
    app.run_window(window.ptr(), 0.0);
    return 0;
}
