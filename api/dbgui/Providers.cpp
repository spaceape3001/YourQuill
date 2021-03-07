////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <db/Cache.hpp>
#include <dbgui/tag/TagProvider.hpp>

namespace provider {
    struct AllTags : public Provider<Tag> {
        AllTags(){}
        ~AllTags(){}
        virtual Vector<Tag>     all() const override { return cdb::all_tags(); }
        virtual bool            can_create() const override { return true; }
        virtual Tag             create(const QString& k, const Root* rt) const override
        {
            return cdb::make_tag(k,rt);
        }
    };
}

