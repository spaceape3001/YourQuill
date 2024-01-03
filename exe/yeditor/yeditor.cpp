////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <0/basic/Logging.hpp>
#include <0/basic/TextUtils.hpp>
#include <0/basic/Url.hpp>
#include <0/sql/SqlLite.hpp>
#include <0/io/Curl.hpp>
#include <0/io/PluginLoader.hpp>
#include <0/meta/Meta.hpp>
#include <0/meta/ObjectInfoWriter.hpp>
#include <tachyon/Application.hpp>
#include <tachyon/Viewer.hpp>
#include <tachyon/ViewerCreateInfo.hpp>
#include <tachyon/Widget.hpp>

#include <mithril/wksp/Workspace.hpp>

#include <imgui.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/attribute/AttributeCDB.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/user/UserCDB.hpp>

using namespace yq;
using namespace yq::mithril;
using namespace yq::tachyon;
using nlohmann::json;

nlohmann::json          gConfig;
std::filesystem::path   gConfigDir;
std::filesystem::path   gConfigFile;

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Explorer : public Widget {
    YQ_OBJECT_DECLARE(Explorer, Widget)
public:
};

YQ_OBJECT_IMPLEMENT(Explorer)

class RealmStatistics : public Widget {
    YQ_OBJECT_DECLARE(RealmStatistics, Widget)
public:

    RealmStatistics(){}
    ~RealmStatistics(){}
    
    struct RowCDB {
        const char* z   = nullptr;
        uint64_t    v   = 0;
    };
    
    void   imgui_(ViContext&u) override
    {
        RowCDB     cacheData[] = {
            { "Atoms", cdb::count_atoms() },
            { "Categories", cdb::all_categories_count() },
            { "Classes", cdb::all_classes_count() },
            { "Directories", cdb::all_directories_count() },
            { "Documents", cdb::all_documents_count() },
            { "Fields", cdb::all_fields_count() },
            { "Fragments", cdb::all_fragments_count() },
            { "Images", cdb::all_images_count() },
            { "Leafs", cdb::all_leafs_count() },
            { "Tags", cdb::count_tags() },
            { "Users", cdb::all_users_count() }
        };
    
        using namespace ImGui;
        Begin("Statistics");
        if(BeginTable("Cache DB", 2)){
            for(RowCDB& r : cacheData){
                TableNextRow();
                TableNextColumn();
                Text(r.z);
                TableNextColumn();
                Text("%u", (unsigned int) r.v);
            }
            EndTable();
        }
        End();
    }
};

YQ_OBJECT_IMPLEMENT(RealmStatistics)

class ClassStatistics : public Widget {
    YQ_OBJECT_DECLARE(ClassStatistics, Widget)
public:

    ClassStatistics(){}
    ~ClassStatistics(){}
    
    void   imgui_(ViContext&u) override
    {
        std::vector<Class>  classes = cdb::all_classes(Sorted::YES);
        
        using namespace ImGui;
        Begin("Statistics");
        if(BeginTable("Cache DB", 2)){
            for(Class c : classes){
                TableNextRow();
                TableNextColumn();
                
                std::string     k   = cdb::label(c);
                Text(k.c_str());
                TableNextColumn();
                
                unsigned int    v   = (unsigned int) cdb::count_atoms(c);
                Text("%u", v);
            }
            EndTable();
        }
        End();
    }
};

YQ_OBJECT_IMPLEMENT(ClassStatistics)

class MainWindow : public Widget {
    YQ_OBJECT_DECLARE(MainWindow, Widget)
public:

    MainWindow()
    {
    }
    
    ~MainWindow()
    {
    }
    
    void   imgui_(ViContext&u) override
    {
        using namespace ImGui;
        
        if(BeginMainMenuBar()){
            if(BeginMenu("Realm")){
                
                if(MenuItem("Statistics")){
                    add_child(new RealmStatistics);
                }
            
                if(MenuItem("Claases")){
                    add_child(new ClassStatistics);
                }
            
                EndMenu();
            }
            
        
            EndMainMenuBar();
        }

        ShowDemoWindow();
        
        Widget::imgui_(u);
    }
    
//    bool    edit(const std::filesystem::path& pth);

    static std::string  make_title(unsigned n=0)
    {
        std::ostringstream  oss;
        oss << "Your Quill: " << wksp::name() << " [" << wksp::author() << "]";
        if(n)
            oss << ": " << n;
        return oss.str();
    }


    std::string     m_status    = "Detangling Photons";
};

YQ_OBJECT_IMPLEMENT(MainWindow)

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool    configure(int argc, char* argv[])
{
    if(argc < 2){
        yCritical() << "Insufficient arguments, expecting one... the port number";
        return false;
    }
    
    unsigned long   pp  = strtoul(argv[1], nullptr, 10);
    if(((pp < 1024) && (pp != 80) && (pp != 443)) || (pp > 49151)){
        yCritical() << "Invalid port number";
        return false;
    }
    
    auto    hdir    = BasicApp::user_home();
    if(!hdir.empty()){
        gConfigDir  = hdir / ".yquill";
        gConfigFile = gConfigDir / "config";
        
        if(std::filesystem::exists(gConfigFile)){
            std::ifstream    fin(gConfigFile);
            gConfig     = nlohmann::json::parse(fin);
        }
    }
    
    Url     u;
    u.scheme    = "http";
    u.host      = "localhost";
    u.path      = "/api/wksp/quill";
    u.port      = (uint16_t) pp;
    
    Curl        contact;
    contact.set_url(u);
    
    HttpStatus hs   = contact.exec();
    if(!isSuccessful(hs)){
        yCritical() << "Unable to query workspace: " << hs.value() << " (" << statusMessage(hs) << ")";
        return false;
    }
   
    nlohmann::json  j   = contact.rx_json();
    std::string where   = j["quill"].get<std::string>();
    if(where.empty()){
        yCritical() << "No meaningful workspace discovered!";
        return false;
    }
    
    wksp::Config        cfg;
    cfg.spec        = where;
    cfg.app         = wksp::App::EDITOR;
    cfg.db_flags    = []() -> int { return SqlLite::ReadOnly; };
    if(!wksp::initialize(cfg))
        return false;
    
    load_plugin_dir("plugin");
    return true;
}

//  MOTTO:  Detangling Photons


int main(int argc, char* argv[])
{
    auto aci = std::make_shared<AppCreateInfo>();
    aci->app_name        = "Your Editor";
    aci->view.title      = "Your Quill: ";
    aci->view.clear      = { 0.0, 0.0, 0.0, 1. };
    aci->view.imgui      = true;
    
    Application app(argc, argv, aci);
    if(!configure(argc, argv))
        return -1;
    app.finalize();
        
    aci->view.title     = MainWindow::make_title();
    app.add_viewer(new MainWindow);
    app.run();
    return 0;
}
