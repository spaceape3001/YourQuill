////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <mithril/atom/Atom.hpp>
#include <basic/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_atoms()
    {
        Command::reg("Atom Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Atom>(ALL);
            w -> setWindowTitle("Atoms");
            w -> model() -> addColumns(DEFAULT);
            w -> model() -> reload();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_atoms();)
}


