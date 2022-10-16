////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uLeaf.hpp"

#include <basic/Logging.hpp>
#include <kernel/atom/AtomCDB.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/leaf/LeafCDB.hpp>
#include <kernel/leaf/LeafData.hpp>

namespace yq {
    namespace update {
        void    leaf_erase(Leaf x)
        {
            static thread_local cdb::SQ  stmts[] = {
                cdb::SQ( "DELETE FROM LTags WHERE leaf=?" ),
                cdb::SQ( "DELETE FROM Leafs WHERE id=?" )
            };
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        void    leaf_erase(Document doc)
        {
            leaf_erase(cdb::leaf(doc));
        }
        
        void    leaf_icon(Leaf x)
        {
            if(!x)
                return ;
                
            Document    doc     = cdb::document(x);
            Image       img     = cdb::best_image(doc);
            static thread_local cdb::SQ u1("UPDATE Leafs SET icon=? WHERE id=?");
            static thread_local cdb::SQ u2("UPDATE Documents SET icon=? WHERE id=?");

            if(cdb::icon(x) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        void    leaf_notify_icons(Fragment frag, Change)
        {
            leaf_icon(cdb::leaf(cdb::document(frag), true));
        }
        
        void    leaf_stage3_pass1_declare(Document doc)
        {
            Leaf    xl  = cdb::db_leaf(doc);
            Atom    xa  = cdb::db_atom(doc);
            
            //  do more
        }
    }
}
