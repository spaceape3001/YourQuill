////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include <mithrilQt/atom/AtomTable.hpp>
#include <basic/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_atoms()
    {
        Command::reg("Atom Table", [](DreamMW* mw){
            mw->addWindow(new AtomTable(ALL));
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_atoms();)
}


