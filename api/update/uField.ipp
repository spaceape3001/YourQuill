////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uField.hpp"
#include <basic/Logging.hpp>
#include <kernel/atom/FieldCDB.hpp>
#include <kernel/atom/FieldData.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/FragmentCDB.hpp>

namespace yq {
    namespace update {
        void    field_erase(Field x)
        {
            static thread_local cdb::SQ stmts[] = {
                cdb::SQ("DELETE FROM CFields WHERE field=?"),
                cdb::SQ("DELETE FROM FTags WHERE field=?"),
                cdb::SQ("DELETE FROM Fields WHERE id=?")
            };

            if(!x)
                return ;
            
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        void    field_erase(Document doc)
        {
            field_erase(cdb::field(doc));
        }
    
        void    field_icon(Field x)
        {
            if(!x)
                return ;
                
            Document    doc     = cdb::document(x);
            Image       img     = cdb::best_image(doc);
            
            static thread_local cdb::SQ u1("UPDATE Fields SET icon=? WHERE id=?");
            static thread_local cdb::SQ u2("UPDATE Documents SET icon=? WHERE id=?");

            if(cdb::icon(x) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        void    field_notify_icons(Fragment frag, Change)
        {
            field_icon(cdb::field(cdb::document(frag), true));
        }

        void    field_stage3_pass1_declare(Document doc)
        {
            cdb::db_field(doc);
        }
    }
}
