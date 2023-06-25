////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <mithril/user/User.hpp>
#include <basic/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_users()
    {
        Command::reg("User Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<User>(ALL);
            w -> setWindowTitle("Users");
            w -> model() -> addColumns(DEFAULT);
            w -> model() -> reload();
            mw->addWindow(w);
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_users();)
}


