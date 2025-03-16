////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <source_location>
#include <vector>

namespace yq::mithril {


    /*! Stage Two Initailization Registration
    
        Stage Two initialization are hooks for initialization that exist *AFTER* the documents & folders
        have been established in the database, but before we do the "on_startup" notifications have been called.
    */
    class Stage2 {
    public:
        static const std::vector<const Stage2*>&    all();
        
        virtual void    invoke() const = 0;
        
        int                 order() const { return m_order; }
        const std::source_location&    source() const { return m_source; }
        
    protected:
        Stage2(int order, const std::source_location&);
        ~Stage2();
        
    private:
        struct Repo;
        static Repo& repo();
        
        int                     m_order;
        std::source_location    m_source;
    };
    
    template <void (*FN)()>
    class SimpleStage2Adapter : public Stage2 {
    public:
    
        SimpleStage2Adapter(int _order, const std::source_location& sl) : Stage2(_order, sl)
        {
        }
        
    
        void    invoke() const override
        {
            FN();
        }
    };

    template <void (*FN)()>
    void    on_stage2(int order=0, const std::source_location&sl = std::source_location::current())
    {
        new SimpleStage2Adapter<FN>(order, sl);
    }
}
