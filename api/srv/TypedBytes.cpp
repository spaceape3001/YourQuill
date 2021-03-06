////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TypedBytes.hpp"
#include <db/FileSys.hpp>
#include <db/Workspace.hpp>

TypedBytes::TypedBytes(std::initializer_list<QString> keys, ContentType ct)
{
    if(wksp::has_init()){
        for(QString k : keys){
            Document    a   = cdb::document(k);
            if(!a)
                continue;
            Fragment   f    = cdb::first(a);
            if(!f)   
                continue;
            content     = cdb::bytes(f);
            type        = mimeTypeForExt(cdb::suffix(a));
            if(type == ContentType())
                type    = ct;
            break;
        }
    }
}

TypedBytes::TypedBytes(DataRole dr, std::initializer_list<QString> keys, ContentType ct)
{
    if(wksp::has_init()){
        for(QString k : keys){
            Document    a   = cdb::document(k);
            if(!a)
                continue;
            Fragment   f    = cdb::first(a);
            if(!f)   
                continue;
            content     = cdb::bytes(f);
            type        = mimeTypeForExt(cdb::suffix(a));
            if(type == ContentType())
                type    = ct;
            break;
        }
    }
}

