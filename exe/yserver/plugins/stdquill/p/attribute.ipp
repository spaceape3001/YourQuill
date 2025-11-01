////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/attribute/AttributeJson.hpp>
#include <yq/mithril/attribute/AttributeSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_attribute(WebContext& ctx)
        {
            Attribute    v   = arg::attribute(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_attribute_key(WebContext& ctx)
        {
            Attribute    v   = arg::attribute(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_attributes(WebContext& ctx)
        {
            AttributeVector  ret = search(ctx, ATTRIBUTE);
            return json{
                { "attributes", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_attribute(WebHtml& h)
        {
            Attribute a = attribute(h);
            if(!a)
                throw HttpStatus::BadArgument;
            h.title() << "Attribute " << a.id;
            auto t = h.table();
            h.kvrow("Atom") << a.id;
            h.kvrow("Field") << dev(field(a));
        }

        void    p_dev_attributes(WebHtml&h)
        {
            h.title() << "All Attributes";
            dev_table(h, all_attributes());
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_attribute_pages()
        {
            reg_webpage<p_api_attribute>("/api/attribute").argument("ID", "Attribute ID");
            reg_webpage<p_api_attribute_key>("/api/attribute/key").argument("ID", "Attribute ID");
            reg_webpage<p_api_attributes>("/api/attributes");

            reg_webpage<p_dev_attribute>("/dev/attribute");
            reg_webpage<p_dev_attributes>("/dev/attributes");
        }
}
