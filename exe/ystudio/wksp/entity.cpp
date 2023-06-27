////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <mithril/entity/Entity.hpp>
#include <basic/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_entitys()
    {
        Command::reg("Entity Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Entity>(ALL);
            w -> setWindowTitle("Entities");
            w -> model() -> addColumns(DEFAULT);
            w -> refresh();
            w -> finalize();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_entitys();)
}


