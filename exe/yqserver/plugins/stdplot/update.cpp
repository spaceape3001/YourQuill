////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/DelayInit.hpp>
#include <basic/Logging.hpp>

#include <aether/kernel/atom/AtomCDB.hpp>
#include <aether/kernel/atom/Class.hpp>
#include <aether/kernel/atom/ClassCDB.hpp>

#include <aether/kernel/notify/AtomNotifier.hpp>
#include <aether/kernel/notify/FileWatch.hpp>
#include <aether/kernel/notify/FileNotifyAdapters.hpp>
#include <aether/kernel/notify/Stage2.hpp>
#include <aether/kernel/notify/Stage3.hpp>
#include <aether/kernel/notify/Stage4.hpp>

using namespace yq;

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

