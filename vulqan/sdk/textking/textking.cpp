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

const char* szSpeech = "No man thinks more highly than I do of the patriotism, as well as abilities, of the very worthy gentlemen who have just addressed the House. But different men often see the same subject in different lights; and, therefore, I hope it will not be thought disrespectful to those gentlemen if, entertaining as I do opinions of a character very opposite to theirs, I shall speak forth my sentiments freely and without reserve. This is no time for ceremony. The questing before the House is one of awful moment to this country. For my own part, I consider it as nothing less than a question of freedom or slavery; and in proportion to the magnitude of the subject ought to be the freedom of the debate. It is only in this way that we can hope to arrive at truth, and fulfill the great responsibility which we hold to God and our country. Should I keep back my opinions at such a time, through fear of giving offense, I should consider myself as guilty of treason towards my country, and of an act of disloyalty toward the Majesty of Heaven, which I revere above all earthly kings.\
\
Mr. President, it is natural to man to indulge in the illusions of hope. We are apt to shut our eyes against a painful truth, and listen to the song of that siren till she transforms us into beasts. Is this the part of wise men, engaged in a great and arduous struggle for liberty? Are we disposed to be of the number of those who, having eyes, see not, and, having ears, hear not, the things which so nearly concern their temporal salvation? For my part, whatever anguish of spirit it may cost, I am willing to know the whole truth; to know the worst, and to provide for it. I have but one lamp by which my feet are guided, and that is the lamp of experience. I know of no way of judging the future but by the past. And judging by the past, I wish to know what there has been in the conduct of the British ministry for the last ten years to justify those hopes with which gentlemen have been pleased to solace themselves and the House. Is it that insidious smile with which our petition has been lately received? Trust it not, sir; it will prove a snare to your feet. Suffer not yourselves to be betrayed with a kiss. Ask yourselves how this gracious reception of our petition comports with those warlike preparations which cover our waters and darken our land. Are fleets and armies necessary to a work of love and reconciliation? Have we shown ourselves so unwilling to be reconciled that force must be called in to win back our love? Let us not deceive ourselves, sir. These are the implements of war and subjugation; the last arguments to which kings resort. I ask gentlemen, sir, what means this martial array, if its purpose be not to force us to submission? Can gentlemen assign any other possible motive for it? Has Great Britain any enemy, in this quarter of the world, to call for all this accumulation of navies and armies? No, sir, she has none. They are meant for us: they can be meant for no other. They are sent over to bind and rivet upon us those chains which the British ministry have been so long forging. And what have we to oppose to them? Shall we try argument? Sir, we have been trying that for the last ten years. Have we anything new to offer upon the subject? Nothing. We have held the subject up in every light of which it is capable; but it has been all in vain. Shall we resort to entreaty and humble supplication? What terms shall we find which have not been already exhausted? Let us not, I beseech you, sir, deceive ourselves. Sir, we have done everything that could be done to avert the storm which is now coming on. We have petitioned; we have remonstrated; we have supplicated; we have prostrated ourselves before the throne, and have implored its interposition to arrest the tyrannical hands of the ministry and Parliament. Our petitions have been slighted; our remonstrances have produced additional violence and insult; our supplications have been disregarded; and we have been spurned, with contempt, from the foot of the throne! In vain, after these things, may we indulge the fond hope of peace and reconciliation. There is no longer any room for hope. If we wish to be free– if we mean to preserve inviolate those inestimable privileges for which we have been so long contending–if we mean not basely to abandon the noble struggle in which we have been so long engaged, and which we have pledged ourselves never to abandon until the glorious object of our contest shall be obtained–we must fight! I repeat it, sir, we must fight! An appeal to arms and to the God of hosts is all that is left us!\
\
They tell us, sir, that we are weak; unable to cope with so formidable an adversary. But when shall we be stronger? Will it be the next week, or the next year? Will it be when we are totally disarmed, and when a British guard shall be stationed in every house? Shall we gather strength by irresolution and inaction? Shall we acquire the means of effectual resistance by lying supinely on our backs and hugging the delusive phantom of hope, until our enemies shall have bound us hand and foot? Sir, we are not weak if we make a proper use of those means which the God of nature hath placed in our power. The millions of people, armed in the holy cause of liberty, and in such a country as that which we possess, are invincible by any force which our enemy can send against us. Besides, sir, we shall not fight our battles alone. There is a just God who presides over the destinies of nations, and who will raise up friends to fight our battles for us. The battle, sir, is not to the strong alone; it is to the vigilant, the active, the brave. Besides, sir, we have no election. If we were base enough to desire it, it is now too late to retire from the contest. There is no retreat but in submission and slavery! Our chains are forged! Their clanking may be heard on the plains of Boston! The war is inevitable–and let it come! I repeat it, sir, let it come.\
\
It is in vain, sir, to extenuate the matter. Gentlemen may cry, Peace, Peace– but there is no peace. The war is actually begun! The next gale that sweeps from the north will bring to our ears the clash of resounding arms! Our brethren are already in the field! Why stand we here idle? What is it that gentlemen wish? What would they have? Is life so dear, or peace so sweet, as to be purchased at the price of chains and slavery? Forbid it, Almighty God! I know not what course others may take; but as for me, give me liberty or give me death!\
";

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
        m_edit -> set_text(szSpeech);
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
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    Begin("N2Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    //Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, GetTotalLines(),
			//IsOverwrite() ? "Ovr" : "Ins",
			//CanUndo() ? "*" : " ",
			//GetLanguageDefinition().mName.c_str(), m_path.c_str());
    m_edit -> draw();
    End();
    PopStyleVar();
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
