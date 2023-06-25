////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <mithril/class/Class.hpp>
#include <basic/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_class()
    {
        Command::reg("Class Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Class>(ALL);
            w -> setWindowTitle("Classes");
            w -> model() -> addColumns(DEFAULT);
            w -> model() -> reload();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_class();)
}


