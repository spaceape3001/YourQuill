////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <yq/mithril/attribute/Attribute.hpp>
#include <yq/core/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_attributes()
    {
        Command::reg("Attribute Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Attribute>(ALL);
            w -> setWindowTitle("Attributes");
            w -> model() -> addColumns(DEFAULT);
            w -> refresh();
            w -> finalize();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_attributes();)
}


