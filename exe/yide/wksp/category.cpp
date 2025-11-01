////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <yq/mithril/category/Category.hpp>
#include <yq/core/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_categorys()
    {
        Command::reg("Category Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Category>(ALL);
            w -> setWindowTitle("Categories");
            w -> model() -> addColumns(DEFAULT);
            w -> refresh();
            w -> finalize();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_categorys();)
}


