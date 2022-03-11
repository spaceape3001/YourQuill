////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

//#include "util/Http.hpp"
//#include "util/Ref.hpp"

//#include "db/Enums.hpp"
//#include "util/Ref.hpp"
//#include "srv/Session.hpp"
//#include "srv/TLSGlobals.hpp"
#include <srv/TypedBytes.hpp>

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

extern bool                                         g_can_remote;


bool        has_background();
TypedBytes  cur_background();
TypedBytes  cur_index();
QByteArray  cur_footer();
QByteArray  cur_css();
QByteArray  cur_summary();
QString     cur_text_color();
QString     cur_back_color();
Graph       cur_class_dep_graph();

    

