////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FileWatch.hpp"
#include "Notifier.hpp"
#include "NotifyAdapters.hpp"
#include "Stage2.hpp"
#include "Stage3.hpp"
#include "Stage4.hpp"

#include <yq/collection/Vector.hpp>
#include <yq/file/Fragment.hpp>
#include <yq/text/text_utils.hpp>


namespace yq {
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    struct FileWatch::Repo {
        std::vector<const FileWatch*>   all;
    };
    
    FileWatch::Repo& FileWatch::repo()
    {
        static Repo s_repo;
        return s_repo;
    }

    const std::vector<const FileWatch*>& FileWatch::all()
    {
        return repo().all;
    }

    FileWatch::FileWatch(const std::filesystem::path&fp, const std::source_location& sl)
    {
        m_file      = fp;
        m_source    = sl;
        repo().all.push_back(this);
    }
    
    FileWatch::~FileWatch()
    {
    }


    class FunctionalFileWatch : public FileWatch {
    public:
        std::function<void()>   m_function;
        FunctionalFileWatch(std::function<void()> fn, const std::filesystem::path&p, const std::source_location& sl) : 
            FileWatch(p, sl), m_function(fn) 
        {
        }
        
        void handle() const override 
        { 
            m_function(); 
        }
    };

    void on_change(const std::filesystem::path&fp, std::function<void()>fn, const std::source_location& sl)
    {
        new FunctionalFileWatch(fn, fp, sl);
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    struct Notifier::Repo {
        std::vector<const Notifier*>    all;
        EnumMap<Change,Vector<const Notifier*>> byChange;
    };
    
    Notifier::Repo&  Notifier::repo()
    {
        static Repo s_repo;
        return s_repo;
    }

    const std::vector<const Notifier*>&  Notifier::all()
    {
        return repo().all;
    }
    
    const EnumMap<Change,Vector<const Notifier*>>&     Notifier::change_map()
    {
        return repo().byChange;
    }
    

    Notifier::Notifier(Trigger trigger, Flag<Change> changeMask, Folder folder, std::string_view ext, const std::filesystem::path& file, int _order, const std::source_location& sl)
    {
        m_path      = file;
        m_source    = sl;
        m_folder    = folder;
        m_change    = changeMask;
        m_order     = _order;

        bool        is_extension    = starts(ext, "*.");
        if(is_extension)
            ext     = ext.substr(2);
        m_specific  = ext;
        
        switch(trigger){
        case NoTrigger:
        case ByFile:
            break;
        case ByDocument:
        case ByExtension:
        case ByFolderExt:
        case ByFolderDoc:
            m_trigger   = trigger;
            break;
        case SpecName:
            m_trigger   = is_extension ? ByExtension : ByDocument;
            break;
        case SpecFolderName:
            m_trigger   = is_extension ? ByFolderExt : ByFolderDoc;
            break;
        default:
            m_trigger   = NoTrigger;
            break;
        }

        Repo&   _r = repo();
        _r.all.push_back(this);
        for(Change c : Change::all_values())
            if(changeMask.is_set(c))
                _r.byChange[c] << this;
    }
    
    Notifier::~Notifier()
    {
    }

    Notifier::Writer&     Notifier::Writer::description(std::string_view d)
    {
        if(importer)
            importer -> m_description = d;
        return *this;
    }

    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    struct Stage2::Repo {
        Vector<const Stage2*> all;
    };
    
    Stage2::Repo& Stage2::repo()
    {
        static Repo s_ret;
        return s_ret;
    }
    
    const Vector<const Stage2*>& Stage2::all()
    {
        return repo().all;
    }
    

    Stage2::Stage2(int order, const std::source_location& sl) : m_order(order), m_source(sl)
    {
        repo().all << this;
    }
    
    Stage2::~Stage2()
    {
    }


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    struct Stage3::Repo {
        Vector<const Stage3*> all;
    };
    
    Stage3::Repo& Stage3::repo()
    {
        static Repo s_ret;
        return s_ret;
    }
    
    const Vector<const Stage3*>& Stage3::all()
    {
        return repo().all;
    }
    

    Stage3::Stage3(Folder f, std::string_view name, int order, const std::source_location&sl) 
    {
        bool    is_ext  = starts(name, "*.");
        if(is_ext)
            m_specific  = name.substr(2);
        else
            m_specific      = name;
        m_folder        = f;
        m_order         = order;
        m_source        = sl;
        
        if(f != Folder()){
            m_method    = is_ext ? ByFolderExt : ByFolderName;
        } else {
            m_method    = is_ext ? ByExt : ByName;
        }
        
        repo().all << this;
    }
    
    Stage3::~Stage3()
    {
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    struct Stage4::Repo {
        Vector<const Stage4*> all;
    };
    
    Stage4::Repo& Stage4::repo()
    {
        static Repo s_ret;
        return s_ret;
    }
    
    const Vector<const Stage4*>& Stage4::all()
    {
        return repo().all;
    }
    

    Stage4::Stage4(int order, const std::source_location& sl) : m_order(order), m_source(sl)
    {
        repo().all << this;
    }
    
    Stage4::~Stage4()
    {
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    class FunctionalNotifier : public Notifier {
    public:
        std::function<void()>   m_function;
        FunctionalNotifier(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, int _order, std::function<void()> handler, const std::source_location&sl) :
            Notifier(t, ch, folder, ext, fname, _order, sl), m_function(handler)
        {
            
        }
    
        bool change(Fragment, Change) const override
        {
            m_function();
            return true;
        }
        
    };

#if 0
    Notifier::Writer    on_change(const std::filesystem::path&fp, std::function<void()>fn, const std::source_location& sl)
    {
        return Notifier::Writer{new FunctionalNotifier(Notifier::ByFile, all_set<Change>(), Folder(), std::string_view(), fp, 0, fn, sl)};
    }
#endif
    
    Notifier::Writer    on_change(Folder f, std::string_view ext, std::function<void()>fn, const std::source_location& sl)
    {
        return Notifier::Writer{new FunctionalNotifier(Notifier::SpecFolderName, all_set<Change>(), f, ext, std::filesystem::path(), 0, fn, sl)};
    }
    
    Notifier::Writer    on_change(std::string_view ext, std::function<void()>fn, const std::source_location& sl)
    {
        return Notifier::Writer{new FunctionalNotifier(Notifier::SpecName, all_set<Change>(), Folder(), ext, std::filesystem::path(), 0, fn, sl)};
    }


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    class FunctionalStage4 : public Stage4 {
    public:
        std::function<void()>   m_function;
        FunctionalStage4(int _order, std::function<void()> fn, const std::source_location& sl) : Stage4(_order, sl), m_function(fn)
        {
        }
    
        void    invoke() const override
        {
            m_function();
        }
    };

    void    on_stage4(int order, std::function<void()> fn, const std::source_location&sl)
    {
        new FunctionalStage4(order, fn, sl);
    }
    
    void    on_stage4(std::function<void()>fn, const std::source_location&sl)
    {
        new FunctionalStage4(0, fn, sl);
    }
}
