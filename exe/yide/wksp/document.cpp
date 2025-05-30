////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <mithril/document/Document.hpp>
#include <yq/core/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_documents()
    {
        Command::reg("Document Table", [](DreamMW* mw){
            auto * w    = new IdTableT<Document>(ALL);
            w -> setWindowTitle("Documents");
            w -> model() -> addColumns(DEFAULT);
            w -> model() -> reload();
            w -> refresh();
            w -> finalize();
        }).menu("studio");
    }
    
    YQ_INVOKE(reg_documents();)
}


