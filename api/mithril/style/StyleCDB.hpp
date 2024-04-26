////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/atom/Atom.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/style/Style.hpp>
#include <vector>

#define TBL_STYLES   "Styles"

namespace yq::mithril::cdb {

    using StyleFragDoc = std::pair<Fragment,Style::SharedFile>;


    /*! \brief All styles
        \param[in] sorted   Set to YES to have result sorted by key
    */
    StyleVector             all_styles(Sorted sorted=Sorted());
    
    //! \brief  Count of styles
    size_t                      all_styles_count();
    
    //! \brief Brief description of the style
    std::string                 brief(Style);

    /*! \brief Used to create/get a style ID from a document 
    
        \param[in]  doc             Document 
        \param[out] wasCreated      Pointer to boolean for true/false of creation
    */
    Style                        db_style(Document doc, bool*wasCreated=nullptr);

    //Folder                    detail_folder(Style);


    /*! \brief Document for style
    */
    Document                    document(Style);
    
    
    //! \brief Tests for existence of style
    bool                        exists(Style);

    //! \brief Tests for existence of style
    bool                        exists_style(uint64_t);
    
    //! \brief Icon for style
    Image                       icon(Style);
    
    //! \brief One stop info for style
    //! \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
    Style::Info                 info(Style, bool autoKeyToName=false);
    
    //! Key for the style
    std::string                 key(Style);
    
    //! Label for the style
    std::string                 label(Style);
    
    
    
    /*! \brief Merged Style structure
     *  
     *  This FUSES all fragments together for a single, cohesive view of the style.
    */
    Style::SharedData            merged(Style, cdb_options_t opts=0);
    
    //! Title for the style
    std::string                 name(Style);

    /*! \brief Name/Key/Icon for style
    
        This returns the Name, Key, & Icon for a style, using a single SQL statement.
        \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
    */
    NKI                         nki(Style, bool autoKeyToName=false);
    
    //! \brief Returns the FIRST style fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    Style::SharedFile            read(Style, const RootDir*, cdb_options_t opts=0);
    
    /*! \brief Returns ALL style fragments
    
        This returns a vector of ALL style fragments for style.  (No merging)
    */
    std::vector<StyleFragDoc>    reads(Style, cdb_options_t opts=0);

    /*! \brief Returns style fragments for given root_dir
    
        This returns a vector of fragments for given style.  (No merging)
        
        \note this will only be one, at most on a case-insensitive file system.  However, on a case sensitive
        file system, this may be multiple.
    */
    std::vector<StyleFragDoc>    reads(Style, class RootDir*, cdb_options_t opts=0);

    /*! \brief ID to style
     *      
     *  Verifies the ID is valid, and converts it to a style
     */
    Style                        style(uint64_t);
    
    //! \brief Looking up a style by string (case insensitive)
    Style                        style(std::string_view);

    //! \brief Document for an atom
    Style                        style(Document, bool calc=false);
    //Style                        style(Folder);

    /*! \brief Style document
     *  
     *  This loads a SINGLE fragment into a style structure.
     *  \note This may be incomplete as there can be multiple fragments to a style.
     */
    Style::SharedFile            style_doc(Fragment, unsigned int options=0);


    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    Style::SharedFile            write(Style, const RootDir*, cdb_options_t opts=0);

}
