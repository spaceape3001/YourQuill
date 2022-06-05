////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/preamble.hpp>
#include <yq/file/Document.hpp>
#include <yq/file/Folder.hpp>
#include <yq/file/FileSpec.hpp>
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

        const FileSpec&             spec() const { return m_spec; }
        
        //  either extension or name depending on method
        int                             order() const { return m_order; }
        const std::source_location&     source() const { return m_source; }
        
    protected:
        Stage3(const FileSpec& spec, int order, const std::source_location&);
        ~Stage3();
        
    private:
        struct Repo;
        static Repo& repo();
        
        FileSpec                m_spec;
        int                     m_order;
        std::source_location    m_source;
    };
    
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    template <void (*FN)()>
    class NullFNStage3Adapter : public Stage3 {
    public:
        NullFNStage3Adapter(const FileSpec& spec, int _order, const std::source_location& sl) : Stage3(spec, _order, sl)
        {
        }
        
    
        void    invoke(Document doc) const override
        {
            FN();
        }
    };

    template <void (*FN)()>
    void    on_stage3(const FileSpec& spec, const std::source_location&sl = std::source_location::current())
    {
        new NullFNStage3Adapter<FN>(spec, 0, sl);
    }

    template <void (*FN)()>
    void    on_stage3(int order, const FileSpec& spec, const std::source_location&sl = std::source_location::current())
    {
        new NullFNStage3Adapter<FN>(spec, order, sl);
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    template <void (*FN)(Document)>
    class SimpleStage3Adapter : public Stage3 {
    public:
    
        SimpleStage3Adapter(const FileSpec& spec, int _order, const std::source_location& sl) : Stage3(spec, _order, sl)
        {
        }
        
    
        void    invoke(Document doc) const override
        {
            FN(doc);
        }
    };
    
    template <void (*FN)(Document)>
    void    on_stage3(const FileSpec& spec, const std::source_location&sl = std::source_location::current())
    {
        new SimpleStage3Adapter<FN>(spec, 0, sl);
    }

    template <void (*FN)(Document)>
    void    on_stage3(int order, const FileSpec& spec, const std::source_location&sl = std::source_location::current())
    {
        new SimpleStage3Adapter<FN>(spec, order, sl);
    }
}
