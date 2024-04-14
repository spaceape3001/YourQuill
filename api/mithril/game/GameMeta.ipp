////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeInfoWriter.hpp>
#include "GameCDB.hpp"

namespace {
    void reg_game_meta()
    {
        using namespace yq;
        using namespace yq::mithril;
        
        {
            auto w  = writer<Game>();
            w.property("id", READ_ONLY, &Game::id);
            w.property("key", (std::string(*)(Game)) cdb::key);
        }
    }
}
