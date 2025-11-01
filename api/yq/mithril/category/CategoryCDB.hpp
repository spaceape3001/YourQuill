////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/category/Category.hpp>
#include <yq/mithril/category/CategoryData.hpp>
#include <yq/mithril/category/CategoryFile.hpp>

#define TBL_CATEGORIES  "Categories"

namespace yq::mithril::cdb {
    using CatFragDoc      = std::pair<Fragment, Category::SharedFile>;
    
    /*! \brief All categories
        \param[in] sorted   Sort by key
    */
    CategoryVector   all_categories(Sorted sorted=Sorted());
    
    /*! \brief Number of categories
    */
    size_t                  all_categories_count();
    
    //! Brief for category
    std::string             brief(Category);

    /*! \brief Gets category by ID
    */
    Category                category(uint64_t);
    
    /*! \brief Gets category by category document
    */
    Category                category(Document, bool calc=false);
    
    /*! \brief Gets category by key
    */
    Category                category(std::string_view);
    
    /*! \brief Gets Category file for fragment
    */
    Category::SharedFile    category_doc(Fragment, bool fAllowEmpty=false);
    
    /*! \brief All classes under category
    */
    ClassVector      classes(Category);
    
    /*! \brief Gets/Creates category by document
    */
    Category                db_category(Document, bool* was_created=nullptr);
    /*! \brief Gets/Creates category by fragment
    */
    Category                db_category(Fragment f, bool* wasCreated=nullptr);
    /*! \brief Gets/Creates category by key
    */
    Category                db_category(std::string_view k, bool* wasCreated=nullptr);
    
    /*! \brief Document that defines category
    */
    Document                document(Category);
    
    //! \brief Checks for category existance
    bool                    exists(Category);

    //! \brief Checks for category existance
    bool                    exists_category(uint64_t);

    //! All fields under category
    FieldVector      fields(Category);
    
    //! Icon for category
    Image                   icon(Category);
    
    //! One stop info for category
    Category::Info          info(Category, bool autoKeyToName=false);

    //! Key for category
    std::string             key(Category);
    
    //! Label for category
    std::string             label(Category);
    
    //! Makes a category 
    Category                make_category(std::string_view, const RootDir* rt=nullptr, cdb_options_t opts=0, bool* wasCreated=nullptr);
    
    //! FUSED document data for category
    Category::SharedData    merged(Category, cdb_options_t opts=0);
    
    //! Name of the category
    std::string             name(Category);
    
    //! Name, Key, Icon for the category
    NKI                     nki(Category, bool autoKeyToName=false);

    //! \brief Returns the FIRST category fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    Category::SharedFile    read(Category, const RootDir*, cdb_options_t opts=0);
    
    //! All fragments for the category
    std::vector<CatFragDoc> reads(Category, cdb_options_t opts=0);
    //! Fragments for the category under specified root
    std::vector<CatFragDoc> reads(Category, class RootDir*, cdb_options_t opts=0);

    //! Sets the brief for the category
    bool                    set_brief(Category, std::string_view, class RootDir* rt=nullptr);
    
    //! Sets the name for the category
    bool                    set_name(Category, std::string_view, class RootDir* rt=nullptr);


    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    Category::SharedFile    write(Category, const RootDir*, cdb_options_t opts=0);
}
