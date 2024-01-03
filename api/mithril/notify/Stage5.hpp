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

    /*! Stage Five Initialization Registration
    
        Stage Five initialization are hooks that run EVERY scan tick(), after running has started.
        
        \note No more registrations should occur here.
    */
    class Stage5 {
    public:
    
    
        static const std::vector<const Stage5*>&    all();
        
        virtual void    invoke() const = 0;
        
        int                 order() const { return m_order; }
        const std::source_location&    source() const { return m_source; }
        
        using OneTime = std::function<void()>;
        
        //! One time hook, called on next scan's tick()
        static void     push_once(OneTime&&);
        static void     swap_one_times(std::vector<OneTime>&);

    protected:
        Stage5(int order, const std::source_location&);
        ~Stage5();
        
    private:
        struct Repo;
        static Repo& repo();
        
        int                     m_order;
        std::source_location    m_source;
    };
    
    template <void (*FN)()>
    class SimpleStage5Adapter : public Stage5 {
    public:
    
        SimpleStage5Adapter(int _order, const std::source_location& sl) : Stage5(_order, sl)
        {
        }
        
    
        void    invoke() const override
        {
            FN();
        }
    };
    
    template <void (*FN)()>
    void    on_stage5(int order=0, const std::source_location&sl = std::source_location::current())
    {
        new SimpleStage5Adapter<FN>(order, sl);
    }

    void    on_stage5(int order, std::function<void()>, const std::source_location&sl = std::source_location::current());
    void    on_stage5(std::function<void()>, const std::source_location&sl = std::source_location::current());
}
