////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/Http.hpp>


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
class Session;


namespace stefanfrings {
    class HttpRequest;
    class HttpResponse;
};


extern bool                                         &x_can_remote;
extern thread_local stefanfrings::HttpRequest*      &x_request;
extern thread_local stefanfrings::HttpResponse*     &x_response;
extern thread_local QDateTime                       &x_at;
extern thread_local QString                         &x_time;
extern thread_local bool                            &x_is_local;
extern thread_local bool                            &x_can_edit;
extern thread_local QByteArray                      &x_client;
extern thread_local HttpOp                          &x_op;
extern thread_local QByteArray                      &x_path;
extern thread_local QByteArray                      &x_pathinfo;    // path minus the starting slash
extern thread_local Session*                        &x_session;
extern thread_local String                          &x_title;
extern thread_local QByteArray                      &x_content;
extern thread_local Vector<QByteArray>              &x_scripts;

extern thread_local Atom                            &x_atom;
extern thread_local Class                           &x_class;
extern thread_local Directory                       &x_directory;
extern thread_local Document                        &x_document;
extern thread_local Field                           &x_field;
extern thread_local Folder                          &x_folder;
extern thread_local Fragment                        &x_fragment;
extern thread_local Graph                           &x_graph;
extern thread_local Image                           &x_icon;
extern thread_local Leaf                            &x_leaf;
extern thread_local const Page*                     &x_page;
extern thread_local const Root*                     &x_root;
extern thread_local Tag                             &x_tag;
extern thread_local bool                            &x_edit_req;
extern thread_local QString                         &x_key;
extern thread_local unsigned int                    &x_columns;

void        reset_tls();    // one stop to wipe the contents away, replace with defaults
