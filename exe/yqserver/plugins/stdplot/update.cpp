////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/DelayInit.hpp>
#include <basic/Logging.hpp>

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/atom/Class.hpp>
#include <mithril/atom/ClassCDB.hpp>

#include <mithril/notify/AtomNotifier.hpp>
#include <mithril/notify/FileWatch.hpp>
#include <mithril/notify/FileNotifyAdapters.hpp>
#include <mithril/notify/Stage2.hpp>
#include <mithril/notify/Stage3.hpp>
#include <mithril/notify/Stage4.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    Class   cCharacter;
    Class   cEvent;
    
    void    u_characters(const AtomChangeData&acd)
    {
        yInfo() << "Character class detected. [" << cdb::key(acd.atom) << "]";
    }
    
    void    u_events(const AtomChangeData&acd)
    {
        yInfo() << "Event class detected. [" << cdb::key(acd.atom) << "]";
    }

    void    reg_stage2()
    {
        yInfo() << "Plog stage 2";
        cCharacter  = cdb::db_class("Character");
        cEvent   = cdb::db_class("Event");
        on_change<u_characters>(by_class(cCharacter));
        on_change<u_events>(by_class(cEvent));
    }

    void    reg_me()
    {
        on_stage2<reg_stage2>();
    }
    
    YQ_INVOKE(reg_me();)
}

