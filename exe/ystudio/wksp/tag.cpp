////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <mithril/tag/Tag.hpp>
#include <basic/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_tags()
    {
        Command::reg("Tag Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Tag>(ALL);
            w -> setWindowTitle("Tags");
            w -> model() -> addColumns(DEFAULT);
            w -> refresh();
            w -> finalize();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_tags();)
}


