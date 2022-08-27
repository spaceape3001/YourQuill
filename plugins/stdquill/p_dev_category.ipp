////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_dev_categories(WebHtml&h)
    {
        h.title() << "All Categories!";
        dev_table(h, all_categories(Sorted::YES));
    }

    void    page_dev_category(WebHtml& h)
    {
        Category    cat = category(h);
        if(!cat)
            throw HttpStatus::BadArgument;
        Category::Info  i   = info(cat);
        
        h.title() << "Category (" << cdb::label(cat) << ")";
        auto ta = h.table();
        
        h.kvrow("ID") << cat.id;
        h.kvrow("Key") << i.key;
        h.kvrow("Name") << i.name;
        h.kvrow("Brief") << i.brief;
    }
    
    void    page_dev_category_classes(WebHtml& h)
    {
        Category    cat = category(h);
        if(!cat)
            throw HttpStatus::BadArgument;
        
        h.title() << "Category (" << cdb::label(cat) << "): In Classes";
        dev_table(h, classes(cat));
    }

    void    page_dev_category_fields(WebHtml& h)
    {
        Category    cat = category(h);
        if(!cat)
            throw HttpStatus::BadArgument;
        
        h.title() << "Category (" << cdb::label(cat) << "): In Fields";
        dev_table(h, fields(cat));
    }

    void reg_dev_category()
    {
        reg_webpage<page_dev_categories>("/dev/categories"); 
        reg_webgroup({
            reg_webpage<page_dev_category>("/dev/category").argument("id", "Category ID").label("Info"),
            reg_webpage<page_dev_category_classes>("/dev/category/classes").argument("id", "Category ID").label("Classes"),
            reg_webpage<page_dev_category_fields>("/dev/category/fields").argument("id", "Category ID").label("Fields")
        });
    }
}

