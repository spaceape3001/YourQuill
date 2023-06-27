////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <mithril/fragment/Fragment.hpp>
#include <basic/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_fragments()
    {
        Command::reg("Fragment Table", [](DreamMW* mw){
            auto * w    = new IdTableT<Fragment>(ALL);
            w -> setWindowTitle("Fragments");
            w -> model() -> addColumns(DEFAULT);
            w -> refresh();
            w -> finalize();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_fragments();)
}


