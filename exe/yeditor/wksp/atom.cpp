////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include "DreamMW.hpp"
#include "IdTable.hpp"

#include <mithril/atom/Atom.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <yq/core/DelayInit.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
    void    reg_atoms()
    {
        Command::reg("Atom Table", [](DreamMW* mw){
        
            auto * w    = new IdTableT<Atom>(ALL);
            w -> setWindowTitle("Atoms");
            w -> model() -> addColumns(DEFAULT);
            w -> refresh();
            w -> finalize();
            mw->addWindow(w);
        }).menu("studio");

        
        Command::reg("Atoms", [](DreamMW* mw, Id id){
            Class c     = id.as<Class>();
            if(!c)
                return ;
                
            std::string cn  = cdb::label(c);
            QString winTitle    = QString("Atoms using (%1)").arg(QString::fromUtf8(cn.c_str()));
            auto *w = new IdTableT<Atom>([c]() -> std::vector<Atom> {
                return cdb::all_atoms(c);
            });
            w -> setWindowTitle(winTitle);
            w -> model() -> addColumns(DEFAULT);
            w -> refresh();
            w -> finalize();
            mw->addWindow(w);
        }).types({}).type<Class>();
    }
    
    YQ_INVOKE(reg_atoms();)
}


