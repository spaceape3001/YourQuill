////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Stage4.hpp"

namespace yq {

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
