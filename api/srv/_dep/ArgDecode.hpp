////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ArgStrings.hpp"
#include "TLSGlobals.hpp"
#include <stdint.h>
#include <string_view>

namespace yq {

    class Atom;
    class Class;
    class Directory;
    class Document;
    class Field;
    class Folder;
    class Fragment;
    class Graph;
    class Image;
    class Leaf;
    class Page;
    class Root;
    class Tag;
    class String;

    enum class R : uint8_t {
        // parsed and found something
        OKAY        = 0,
        // Empty request (ie, not there or blank)
        EMPTY,
        // Parse error
        PARSE,
        // Internal error
        INTERNAL,
        // Thing not found
        NOT_FOUND
    };

    /*! \brief  Returns TRUE if result is OKAY

        If an error occured, then the HTTP code is thrown.

        \param[in]  r           Result
        \param[in]  fIncEmpty   set to TRUE to not throw on "EMPTY" request
        \return TRUE if present, FALSE if empty (and fIncEmpty is true)
    */
    bool        test(R r, bool fIncEmpty=false);


    static constexpr unsigned int   def_columns_mobile  = 2;
    static constexpr unsigned int   def_columns_normal  = 6;

    /*! \brief Decodes input parameter for an atom

        This decodes the user input request for an atom.  Can be "atom" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R   decode_atom(Atom& r=x_atom);

    /*! \brief Decodes input parameter for an atom
        
        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R   decode_atom(const std::string_view&p, Atom& r=x_atom);


    /*! \brief Decodes input parameter for a class

        This decodes the user input request for a class.  Can be "cls" "class" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R   decode_class(Class& r=x_class);

    /*! \brief Decodes input parameter for a class

        This decodes the user input request for a class. 

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R   decode_class(const std::string_view&p, Class& r=x_class);


    /*! \brief Decodes column count

        This decodes the user input request for a column count, "cols"

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R   decode_columns(unsigned int& r=x_columns);

    /*! \brief Decodes column count

        This decodes the user input request for a column count

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R   decode_columns(const std::string_view& p, unsigned int& r=x_columns);


    /*! \brief Decodes input parameter for a directory

        This decodes the user input request for a directory.  Can be "dir" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R   decode_directory(Directory &r=x_directory);

    /*! \brief Decodes input parameter for a directory

        This decodes the user input request for a directory.  

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R   decode_directory(const std::string_view&p, Directory &r=x_directory);


    //  TODO We're going to four permutations on these, no defaults


    /*! \brief Decodes input parameter for a document

        This decodes the user input request for a document.  Can be "doc" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_document(Document &r=x_document);

    /*! \brief Decodes input parameter for a document

        This decodes the user input request for a document.

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_document(const std::string_view&p, Document&r=x_document);


    /*! \brief Decodes input parameter for a folder

        This decodes the user input request for a folder.  Can be "folder" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_folder(Folder& r=x_folder);

    /*! \brief Decodes input parameter for a folder

        This decodes the user input request for a folder.

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_folder(const std::string_view&p, Folder& r=x_folder);

    /*! \brief Decodes input parameter for a fragment

        This decodes the user input request for a fragment.  Can be "frag" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_fragment(Fragment& r=x_fragment);

    /*! \brief Decodes input parameter for a fragment

        This decodes the user input request for a fragment.

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_fragment(const std::string_view&p, Fragment& r=x_fragment);


    /*! \brief Decodes input parameter for a graph

        This decodes the user input request for a graph.  Can be "graph" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_graph(Graph& r=x_graph);

    /*! \brief Decodes input parameter for a graph

        This decodes the user input request for a graph.

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_graph(const std::string_view&p, Graph& r=x_graph);

    /*! \brief Decodes input parameter for a icon

        This decodes the user input request for a icon.  Can be "icon" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_icon(Image& r=x_icon);

    /*! \brief Decodes input parameter for a icon

        This decodes the user input request for a icon.

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_icon(const std::string_view&p, Image& r=x_icon);

    /*! \brief Decodes input parameter for a leaf

        This decodes the user input request for a leaf.  Can be "leaf" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_leaf(Leaf& r=x_leaf);

    /*! \brief Decodes input parameter for a leaf

        This decodes the user input request for a leaf.

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_leaf(const std::string_view&p, Leaf& r=x_leaf);

    /*! \brief Decodes input string for a leaf ID/key

        This decodes the user input request for a leaf.

        \param[out] r   Result goes here
        \return         Success/failure
    */

    R     decode_leaf_key(const std::string_view& k, Leaf& r=x_leaf);


    /*! \brief Decodes input parameter for a root

        This decodes the user input request for a root.  Can be "root" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_root(const Root*& r=x_root);

    /*! \brief Decodes input parameter for a root

        This decodes the user input request for a root.

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_root(const std::string_view&p, const Root*& r=x_root);

    /*! \brief Decodes input parameter for a tag

        This decodes the user input request for a tag.  Can be "tag" "id" or "key".

        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_tag(Tag& r=x_tag);

    /*! \brief Decodes input parameter for a tag

        This decodes the user input request for a tag.

        \param[in] p    Query key in the user inputed parameters
        \param[out] r   Result goes here
        \return         Success/failure
    */
    R     decode_tag(const std::string_view&p, Tag& r=x_tag);




    R     decode_edit();
    R     decode_key();
}
