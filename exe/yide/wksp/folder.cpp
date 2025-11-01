////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <yq/mithril/folder/Folder.hpp>
#include <yq/core/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_folders()
    {
        Command::reg("Folder Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Folder>(ALL);
            w -> setWindowTitle("Folders");
            w -> model() -> addColumns(DEFAULT);
            w -> refresh();
            w -> finalize();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_folders();)
}


