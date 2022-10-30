////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    p_dev_field(WebHtml& h)
    {
        Field fld = field(h);
        if(!fld)
            throw HttpStatus::BadArgument;
        Field::Info i   = info(fld);
        h.title() << "Field (" << cdb::label(fld) << ")";
        auto ta = h.table();
        h.kvrow("ID") << fld.id;
        h.kvrow("Key") << i.key;
        h.kvrow("Class") << dev(i.class_);
        h.kvrow("Name") << i.name;
        h.kvrow("Plural") << i.plural;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Category") << dev(i.category);
    }
    
    void    p_dev_field_tags(WebHtml& h)
    {
        Field fld = field(h);
        if(!fld)
            throw HttpStatus::BadArgument;
        h.title() << "Field (" << cdb::label(fld) << "): Tags";
        dev_table(h, tags(fld, Sorted::YES));
    }

    void    p_dev_fields(WebHtml& h)
    {
        h.title() << "All Fields";
        dev_table(h, all_fields(Sorted::YES));
    }

    void    reg_dev_field()
    {
        reg_webgroup({
            reg_webpage<p_dev_field>("/dev/field").argument("id", "Field ID").label("Info"),
            reg_webpage<p_dev_field_tags>("/dev/field/tags").argument("id", "Field ID").label("Fields")
        });
        reg_webpage<p_dev_fields>("/dev/fields");
    }
}
