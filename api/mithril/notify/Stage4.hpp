////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/preamble.hpp>
#include <source_location>
#include <functional>

namespace yq::mithril {

    /*! Stage Four Initailization Registration
    
        Stage Four initialization are hooks for initialization that exist *AFTER* the documents & folders
        have been established in the database, and after the "on_startup" notifications have been called,
        but before we enter stage 5 running.

        \note Stage 4 can be added in stage 3 handlers.  Stage 4 can also register notifiers
    */
    class Stage4 {
    public:
        static const std::vector<const Stage4*>&    all();
        
        virtual void    invoke() const = 0;
        
        int                 order() const { return m_order; }
        const std::source_location&    source() const { return m_source; }
        
    protected:
        Stage4(int order, const std::source_location&);
        ~Stage4();
        
    private:
        struct Repo;
        static Repo& repo();
        
        int                     m_order;
        std::source_location    m_source;
    };
    
    template <void (*FN)()>
    class SimpleStage4Adapter : public Stage4 {
    public:
    
        SimpleStage4Adapter(int _order, const std::source_location& sl) : Stage4(_order, sl)
        {
        }
        
    
        void    invoke() const override
        {
            FN();
        }
    };
    
    template <void (*FN)()>
    void    on_stage4(int order=0, const std::source_location&sl = std::source_location::current())
    {
        new SimpleStage4Adapter<FN>(order, sl);
    }

    void    on_stage4(int order, std::function<void()>, const std::source_location&sl = std::source_location::current());
    void    on_stage4(std::function<void()>, const std::source_location&sl = std::source_location::current());
}
