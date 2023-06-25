////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <mithril/place/Place.hpp>
#include <basic/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_places()
    {
        Command::reg("Place Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Place>(ALL);
            w -> setWindowTitle("Places");
            w -> model() -> addColumns(DEFAULT);
            w -> model() -> reload();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_places();)
}


