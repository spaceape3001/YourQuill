////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//#include <0/basic/CollectionUtils.hpp>
#include <yq/core/DelayInit.hpp>
#include <yq/file/DirUtils.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/core/Logging.hpp>
#include <yq/core/StreamOps.hpp>


#include <yq/stream/Text.hpp>

#include <yq/mithril/html/HtmlLayout.hpp>

#include <yq/mithril/web/JsonAdapter.hpp>
#include <yq/mithril/web/WebAdapters.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/web/WebHtml.hpp>
#include <yq/mithril/web/WebImage.hpp>
#include <yq/mithril/web/WebPage.hpp>
#include <yq/mithril/web/WebRedirect.hpp>
#include <yq/mithril/web/WebTemplate.hpp>
#include <yq/mithril/web/Template.hpp>

#include <yq/mithril/book.hpp>
#include <yq/mithril/character.hpp>
#include <yq/mithril/entity.hpp>
#include <yq/mithril/event.hpp>
#include <yq/mithril/game.hpp>
#include <yq/mithril/group.hpp>
#include <yq/mithril/organization.hpp>
#include <yq/mithril/place.hpp>
#include <yq/mithril/thing.hpp>

using namespace yq;
//using namespace yq::arg;
using namespace yq::mithril;
using namespace yq::mithril::cdb;
using namespace yq::mithril::html;

#include "p/book.ipp"
#include "p/character.ipp"
#include "p/entity.ipp"
#include "p/event.ipp"
#include "p/game.ipp"
#include "p/group.ipp"
#include "p/organization.ipp"
#include "p/place.ipp"
#include "p/thing.ipp"

namespace {

    void    reg_me()
    {
        reg_book_pages();
        reg_character_pages();
        reg_entity_pages();
        reg_event_pages();
        reg_game_pages();
        reg_group_pages();
        reg_organization_pages();
        reg_place_pages();
        reg_thing_pages();

    
        yInfo() << "Standard plot pages registered";
    }
    
    YQ_INVOKE(reg_me();)
}

