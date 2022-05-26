////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/preamble.hpp>
#include <yq/file/Document.hpp>
#include <yq/file/Folder.hpp>
#include <source_location>
#include <filesystem>

namespace yq {


    /*! Stage Three Initailization Registration
    
        Stage three is the former "on_startup", where it'll probe for each document that exists.  
        
        \note stage 3 can be added in stage 2 handlers.
    */
    class Stage3 {
    public:
        static const Vector<const Stage3*>&    all();
        
        virtual void    invoke(Document) const = 0;
        
        enum Method {
            ByName,
            ByExt,
            ByFolderExt,
            ByFolderName
        };
        
        Method                          method() const { return m_method; }
        Folder                          folder() const { return m_folder; }
        //  either extension or name depending on method
        std::string_view                specific() const { return m_specific; }
        int                             order() const { return m_order; }
        const std::source_location&     source() const { return m_source; }
        
    protected:
        Stage3(Folder, std::string_view sp, int order, const std::source_location&);
        ~Stage3();
        
    private:
        struct Repo;
        static Repo& repo();
        
        Method                  m_method;
        Folder                  m_folder;
        std::string_view        m_specific;
        int                     m_order;
        std::source_location    m_source;
    };
    
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    template <void (*FN)()>
    class NullFNStage3Adapter : public Stage3 {
    public:
        NullFNStage3Adapter(Folder f, std::string_view sp, int _order, const std::source_location& sl) : Stage3(f, sp, _order, sl)
        {
        }
        
    
        void    invoke(Document doc) const override
        {
            FN();
        }
    };

    template <void (*FN)()>
    void    on_stage3(std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        new NullFNStage3Adapter<FN>(Folder(), name, 0, sl);
    }

    template <void (*FN)()>
    void    on_stage3(Folder f, std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        new NullFNStage3Adapter<FN>(f, name, 0, sl);
    }

    template <void (*FN)()>
    void    on_stage3(int order, std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        new NullFNStage3Adapter<FN>(Folder(), name, order, sl);
    }

    template <void (*FN)()>
    void    on_stage3(int order, Folder f, std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        new NullFNStage3Adapter<FN>(f, name, order, sl);
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    template <void (*FN)(Document)>
    class SimpleStage3Adapter : public Stage3 {
    public:
    
        SimpleStage3Adapter(Folder f, std::string_view sp, int _order, const std::source_location& sl) : Stage3(f, sp, _order, sl)
        {
        }
        
    
        void    invoke(Document doc) const override
        {
            FN(doc);
        }
    };
    
    template <void (*FN)(Document)>
    void    on_stage3(std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        new SimpleStage3Adapter<FN>(Folder(), name, 0, sl);
    }

    template <void (*FN)(Document)>
    void    on_stage3(Folder f, std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        new SimpleStage3Adapter<FN>(f, name, 0, sl);
    }

    template <void (*FN)(Document)>
    void    on_stage3(int order, std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        new SimpleStage3Adapter<FN>(Folder(), name, order, sl);
    }

    template <void (*FN)(Document)>
    void    on_stage3(int order, Folder f, std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        new SimpleStage3Adapter<FN>(f, name, order, sl);
    }
}
