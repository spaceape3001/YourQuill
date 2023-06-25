////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <mithril/organization/Organization.hpp>
#include <basic/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_organizations()
    {
        Command::reg("Organization Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Organization>(ALL);
            w -> setWindowTitle("Organizations");
            w -> model() -> addColumns(DEFAULT);
            w -> model() -> reload();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_organizations();)
}


