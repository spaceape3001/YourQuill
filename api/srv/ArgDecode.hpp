////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ArgStrings.hpp"
#include <util/TypeFwd.hpp>

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

//  Returns TRUE if result is OKAY
bool        test(R, bool fIncEmpty=false);


static constexpr unsigned int   def_columns_mobile  = 2;
static constexpr unsigned int   def_columns_normal  = 6;

R     decode_atom(const char* sz=sz_atom);
R     decode_atom(Atom&,const char* sz=sz_atom);

R     decode_atom_prime();

R     decode_class(const char* sz=sz_class);
R     decode_class(Class&, const char* sz=sz_class);
R     decode_class_prime();

R     decode_columns();

R     decode_directory(const char* sz=sz_directory);
R     decode_directory(Directory&, const char* sz=sz_directory);
R     decode_directory_prime();

R     decode_document(const char* sz=sz_document);
R     decode_document(Document&, const char* sz=sz_document);
R     decode_document_prime();

R     decode_folder(const char* sz=sz_folder);
R     decode_folder(Folder&, const char* sz=sz_folder);
R     decode_folder_prime();

R     decode_fragment(const char* sz=sz_fragment);
R     decode_fragment(Fragment&, const char* sz=sz_fragment);
R     decode_fragment_prime();

R     decode_graph(const char* sz=sz_folder);
R     decode_graph(Graph&, const char* sz=sz_folder);
R     decode_graph_prime();

R     decode_icon(const char* sz=sz_icon);
R     decode_icon(Image&, const char* sz=sz_icon);
R     decode_icon_prime();

R     decode_leaf(const char* sz=sz_leaf);
R     decode_leaf(Leaf&, const char* sz=sz_leaf);
R     decode_leaf_prime();

R     decode_leaf_string(Leaf& out, const String&);
R     decode_leaf_prime(const String&);

R     decode_root(const char* sz=sz_root);
R     decode_root(const Root*&, const char* sz=sz_root);
R     decode_root_prime();

R     decode_tag(const char* sz=sz_tag);
R     decode_tag(Tag&, const char* sz=sz_tag);
R     decode_tag_prime();

R     decode_edit();
R     decode_key();
