////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <yq/mithril/doodle/Doodle.hpp>
#include <yq/core/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_doodles()
    {
        Command::reg("Doodle Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Doodle>(ALL);
            w -> setWindowTitle("Doodles");
            w -> model() -> addColumns(DEFAULT);
            w -> refresh();
            w -> finalize();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_doodles();)
}


