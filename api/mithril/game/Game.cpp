////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "GameCDB.hpp"
#include <yq/meta/TypeInfoWriter.hpp>
#include <yq/meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Game)
YQ_TYPE_IMPLEMENT(yq::mithril::GameSet)
YQ_TYPE_IMPLEMENT(yq::mithril::GameVector)

namespace yq::mithril {
    void reg_game_meta()
    {
        {
            auto w  = writer<Game>();
            w.property("id", READ_ONLY, &Game::id);
            w.property("key", (std::string(*)(Game)) cdb::key);
        }
    }

    YQ_INVOKE(reg_game_meta();)
}
