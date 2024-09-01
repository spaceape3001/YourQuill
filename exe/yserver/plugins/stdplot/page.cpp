////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//#include <0/basic/CollectionUtils.hpp>
#include <yq/basic/DelayInit.hpp>
#include <0/io/DirUtils.hpp>
#include <0/io/FileUtils.hpp>
#include <yq/basic/Logging.hpp>
#include <0/io/StreamOps.hpp>
#include <0/basic/TextUtils.hpp>

#include <yq/io/stream/Text.hpp>

#include <mithril/html/HtmlLayout.hpp>

#include <mithril/web/JsonAdapter.hpp>
#include <mithril/web/WebAdapters.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/web/WebImage.hpp>
#include <mithril/web/WebPage.hpp>
#include <mithril/web/WebRedirect.hpp>
#include <mithril/web/WebTemplate.hpp>
#include <mithril/web/Template.hpp>

#include <mithril/book.hpp>
#include <mithril/character.hpp>
#include <mithril/entity.hpp>
#include <mithril/event.hpp>
#include <mithril/game.hpp>
#include <mithril/group.hpp>
#include <mithril/organization.hpp>
#include <mithril/place.hpp>
#include <mithril/thing.hpp>

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

