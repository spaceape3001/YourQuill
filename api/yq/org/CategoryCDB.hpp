////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/CacheFwd.hpp>
#include <yq/enum/Sorted.hpp>
#include <yq/file/Document.hpp>
#include <yq/image/Image.hpp>
#include <yq/org/Category.hpp>
#include <yq/org/CategoryData.hpp>
#include <yq/org/CategoryFile.hpp>

namespace yq {

    struct Document;
    struct Fragment;
    struct Field;
    struct Class;
    
    struct Category::Info {
        std::string key;
        Document    doc;
        Image       icon;
        std::string name;
        std::string brief;

        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        using CatFragDoc      = std::pair<Fragment, Category::SharedFile>;
        
        std::vector<Category>   all_categories(Sorted sorted=Sorted());
        size_t                  all_categories_count();
        
        Category                category(uint64_t);
        Category                category(Document, bool calc=false);
        Category                category(std::string_view);
        
        Category::SharedFile    category_doc(Fragment, bool fAllowEmpty=false);
        
        std::vector<Class>      classes(Category);
        
        Category                db_category(Document, bool* was_created=nullptr);
        Category                db_category(Fragment f, bool* wasCreated);
        Category                db_category(std::string_view k, bool* wasCreated);
        
        Document                document(Category);
        
        //! Erases the category from the cache
        void                    erase(Category);
        
        bool                    exists(Category);
        bool                    exists_category(uint64_t);

        std::vector<Field>      fields(Category);
        
        Image                   icon(Category);
        
        Category::Info          info(Category, bool autoKeyToName=false);

        std::string             key(Category);
        
        std::string             label(Category);
        
        Category::SharedData    merged(Category, cdb_options_t opts=0);
        
        std::string             name(Category);
        
        NKI                     nki(Category, bool autoKeyToName=false);
    
        //! \brief Returns the FIRST category fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Category::SharedFile    read(Category, const Root*, cdb_options_t opts=0);
        
        std::vector<CatFragDoc> reads(Category, cdb_options_t opts=0);
        std::vector<CatFragDoc> reads(Category, class Root*, cdb_options_t opts=0);

        bool                    set_brief(Category, std::string_view, class Root* rt=nullptr);
        bool                    set_name(Category, std::string_view, class Root* rt=nullptr);

        Category::SharedData    update(Category, cdb_options_t opts=0);
        void                    update_icon(Category);

        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Category::SharedFile    write(Category, const Root*, cdb_options_t opts=0);
    }
}