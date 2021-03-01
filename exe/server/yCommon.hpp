#pragma once

#include "util/Http.hpp"
#include "util/Ref.hpp"

#include "db/Enums.hpp"
#include "util/Ref.hpp"

#include <QByteArray>
#include <initializer_list>

class QDateTime;
class QString;
class QByteArray;
class QHostAddress;

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
class Html;
class Session;
class TypedBytes;


namespace stefanfrings {
    class HttpRequest;
    class HttpResponse;
};

struct ID {
    uint64_t        id;
    int64_t         created;
    uint64_t        magic[14];
    
    void            make();
    void            newId();
    QByteArray      net() const;;
    bool            operator==(const ID&b) const;
};

//YQ_ENUM( IconSize, , Small, Medium, Large )

struct Session : public ID, public RefCount {
    QByteArray              user;
    bool                        loggedIn;
    int64_t                     accessed;
    std::atomic<const Root*>    defRoot;
    std::atomic<bool>           autoEdit;
    SizeDesc                    iconSize;
    bool                        inspectSubmit;
    unsigned int                columns;
    
    Session();  // non-trivial
};

//!  Makes/create a session 
Ref<Session>      sessionFor(const QByteArray& decl);




using namespace stefanfrings;

extern bool                             x_can_remote;
extern thread_local HttpRequest*        x_request;
extern thread_local HttpResponse*       x_response;
extern thread_local QDateTime           x_at;
extern thread_local QString             x_time;
extern thread_local bool                x_is_local;
extern thread_local bool                x_can_edit;
extern thread_local QByteArray          x_client;
extern thread_local HttpOp              x_op;
extern thread_local QByteArray          x_path;
extern thread_local Session*            x_session;
extern thread_local QByteArray          x_title;
extern thread_local QByteArray          x_content;
extern thread_local Vector<QByteArray>  x_scripts;

extern thread_local Atom                x_atom;
extern thread_local Class               x_class;
extern thread_local Directory           x_directory;
extern thread_local Document            x_document;
extern thread_local Field               x_field;
extern thread_local Folder              x_folder;
extern thread_local Fragment            x_fragment;
extern thread_local Graph               x_graph;
extern thread_local Image               x_icon;
extern thread_local Leaf                x_leaf;
extern thread_local const Page*         x_page;
extern thread_local const Root*         x_root;
extern thread_local Tag                 x_tag;
extern thread_local bool                x_edit_req;
extern thread_local QString             x_key;
extern thread_local unsigned int        x_columns;

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



static constexpr const char*    sz_atom         = "atom";
static constexpr const char*    sz_class        = "cls";
static constexpr const char*    sz_directory    = "dir";
static constexpr const char*    sz_document     = "doc";
static constexpr const char*    sz_folder       = "folder";
static constexpr const char*    sz_fragment     = "frag";
static constexpr const char*    sz_graph        = "graph";
static constexpr const char*    sz_icon         = "icon";
static constexpr const char*    sz_leaf         = "leaf";
static constexpr const char*    sz_root         = "root";
static constexpr const char*    sz_id           = "id";
static constexpr const char*    sz_key          = "key";
static constexpr const char*    sz_tag          = "tag";
static constexpr const char*    sz_edit         = "edit";
static constexpr const char*    sz_columns      = "cols";

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


struct TypedBytes {
    QByteArray      content;
    ContentType     type;
    
    TypedBytes(){}
    TypedBytes(std::initializer_list<QString>,ContentType ct=ContentType());
    TypedBytes(DataRole, std::initializer_list<QString>,ContentType ct=ContentType());
    
    bool    empty() const { return content.isEmpty(); }
};


bool        has_background();
TypedBytes  cur_background();
TypedBytes  cur_index();
QByteArray  cur_footer();
QByteArray  cur_css();
QByteArray  cur_summary();
QString     cur_text_color();
QString     cur_back_color();
QString     cur_user();
Graph       cur_class_dep_graph();

bool        is_mobile();
const Root* def_root(DataRole dr = DataRole());
bool        auto_edit();
bool        valid_key(const QByteArray&);

bool        inspect_submit();

void        return_to_sender();
SizeDesc    icon_size();


QByteArray  reencode_parameters();
Vector<Class>   get_class_vector(const StringSet& sset);
    

