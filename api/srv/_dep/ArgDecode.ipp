////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    namespace {
        QByteArray  q(const std::string_view& v)
        {
            return QByteArray(v.data(), v.size());
        }
    }

    R   decode_atom(Atom& r)
    {
        String  f   = x_request -> getParameter(sz_id);
        if(!f.isEmpty()){
            bool    ok  = false;
            uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
            if(ok){
                x_atom    = cdb::atom(ut);
                if(!x_atom)
                    return R::NOT_FOUND;
                return R::OKAY;
            } else
                return R::PARSE;
        }
        
        f           = x_request -> getParameter(sz_key);
        if(!f.empty()){
            x_atom    = cdb::atom(f);
            if(!x_atom)
                return R::NOT_FOUND;
        }
        
        return decode_atom(sz_atom, f);
    }



    R   decode_atom(const std::string_view&p, Atom& r)
    {
        r   = Atom{};
        if(!x_request)
            return R::INTERNAL;
        String  f   = x_request -> getParameter(q(p));
        if(f.empty())
            return R::EMPTY;
            
            //  TRY ID first
        auto    ut  = f.to_uint64();
        if(ut.good){
            out = cdb::atom(ut.value);
            if(out)
                return R::OKAY;
        }
        
            //  IF that fails, try KEY
        out = cdb::atom(f);
        return out ? R::OKAY : R::NOT_FOUND;
    }


    #error "MORE"

    R  decode_class(Class& out, const char* sz)
    {
        out = Class();
        if(!x_request)
            return R::INTERNAL;
        QByteArray  f   = x_request -> getParameter(sz);
        if(f.isEmpty())
            return R::EMPTY;
            
        out     = cdb::class_(utf8(f));
        if(out)
            return R::OKAY;
            
        bool    ok  = false;
        uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
        if(!ok)
            return R::NOT_FOUND;
        out    = cdb::class_(ut);
        return out ? R::OKAY : R::NOT_FOUND;
    }

    R  decode_class(const char* sz)
    {
        return decode_class(x_class, sz);
    }

    R  decode_class_prime()
    {
        x_class = Class();
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz_id);
        if(!f.isEmpty()){
            bool    ok  = false;
            uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
            if(ok){
                x_class    = cdb::class_(ut);
                if(!x_class)
                    return R::NOT_FOUND;
                return R::OKAY;
            } else
                return R::PARSE;
        } 
        
        f       = x_request -> getParameter(sz_key);
        if(!f.isEmpty()){
            x_class    = cdb::class_(QString::fromUtf8(f));
            if(!x_class)
                return R::NOT_FOUND;
            return R::OKAY;
        }
        
        return decode_class();
    }

    R     decode_columns()
    {
        x_columns       = (x_session && x_session->columns) ? x_session -> columns : (is_mobile() ? def_columns_mobile : def_columns_normal);
        if(!x_request)
            return R::INTERNAL;
        QByteArray  f   = x_request -> getParameter(sz_columns);
        if(f.isEmpty())
            return R::EMPTY;
        bool    ok  = false;
        unsigned  int   u   = f.toUInt(&ok);
        if(!ok)
            return R::PARSE;
        if(u != 0)
            x_columns   = u;
        return R::OKAY;
    }


    R  decode_directory(Directory& out, const char* sz)
    {
        out = Directory();
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz);
        if(f.isEmpty())
            return R::EMPTY;
            
        out = cdb::directory(utf8(f));
        if(out)
            return R::OKAY;
            
        bool    ok  = false;
        uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
        if(!ok)
            return R::NOT_FOUND;
            
        out    = cdb::directory(ut);
        return out ? R::OKAY : R::NOT_FOUND;
    }

    R  decode_directory(const char* sz)
    {
        return decode_directory(x_directory, sz);
    }


    R  decode_directory_prime()
    {
        x_directory = Directory();
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz_id);
        if(!f.isEmpty()){
            bool    ok  = false;
            uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
            if(ok){
                x_directory    = cdb::directory(ut);
                if(!x_directory)
                    return R::NOT_FOUND;
                return R::OKAY;
            } else
                return R::PARSE;
        } 
        
        f       = x_request -> getParameter(sz_key);
        if(!f.isEmpty()){
            x_directory    = cdb::directory(QString::fromUtf8(f));
            if(!x_directory)
                return R::NOT_FOUND;
            return R::OKAY;
        }
        
        return decode_directory();
    }

    R  decode_document(Document&out, const char*sz)
    {
        out  = Document();
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz);
        if(f.isEmpty())
            return R::EMPTY;
        out      = cdb::document(utf8(f));
        if(out)
            return R::OKAY;
        
        bool    ok  = false;
        uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
        if(!ok)
            return R::NOT_FOUND;
        
        out      = cdb::document(ut);
        return out ? R::OKAY : R::NOT_FOUND;
    }

    R  decode_document(const char*sz)
    {
        return decode_document(x_document, sz);
    }

    R  decode_document_prime()
    {
        x_document  = Document();
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz_id);
        if(!f.isEmpty()){
            bool    ok  = false;
            uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
            if(ok){
                x_document    = cdb::document(ut);
                if(!x_document)
                    return R::NOT_FOUND;
                return R::OKAY;
            } else
                return R::PARSE;
        } 
        
        f       = x_request -> getParameter(sz_key);
        if(!f.isEmpty()){
            x_document    = cdb::document(QString::fromUtf8(f));
            if(!x_document)
                return R::NOT_FOUND;
            return R::OKAY;
        }
        
        return decode_document();
    }

    R  decode_edit()
    {
        x_edit_req  = false;
        if(!x_request)
            return R::INTERNAL;
        QByteArray  f   = x_request -> getParameter(sz_edit);
        if(f.isEmpty()){
            if(x_session)
                x_edit_req  = x_session -> autoEdit;
            return R::EMPTY;
        }
        
        if(f == "later"){
            if(x_session)
                x_session -> autoEdit   = false;
            x_edit_req  = false;
            return R::OKAY;
        }
        
        if(f == "now"){
            if(x_session)
                x_session -> autoEdit   = true;
            x_edit_req  = true;
            return R::OKAY;
        }
        
        return R::PARSE;
    }

    R  decode_folder(Folder&out, const char* sz)
    {
        out    = Folder{};
        if(!x_request)
            return R::INTERNAL;
        QByteArray  f   = x_request -> getParameter(sz);
        if(f.isEmpty())
            return R::EMPTY;
            
        out = cdb::folder(utf8(f));
        if(out)
            return R::OKAY;
            
        bool    ok  = false;
        uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
        if(!ok)
            return R::NOT_FOUND;
        
        out = cdb::folder(ut);
        return out ? R::OKAY : R::NOT_FOUND;
    }

    R  decode_folder(const char* sz)
    {
        return decode_folder(x_folder, sz);
    }

    R  decode_folder_prime()
    {
        x_folder    = Folder{};
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz_id);
        if(!f.isEmpty()){
            bool    ok  = false;
            uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
            if(ok){
                x_folder    = cdb::folder(ut);
                if(!x_folder)
                    return R::NOT_FOUND;
                return R::OKAY;
            } else
                return R::PARSE;
        } 
        
        f       = x_request -> getParameter(sz_key);
        if(!f.isEmpty()){
            x_folder    = cdb::folder(QString::fromUtf8(f));
            if(!x_folder)
                return R::NOT_FOUND;
            return R::OKAY;
        }
        
        return decode_folder();
    }



    R  decode_fragment(Fragment& out, const char* sz)
    {
        out  = Fragment{};
        if(!x_request)
            return R::INTERNAL;
        QByteArray  f   = x_request -> getParameter(sz);
        if(f.isEmpty())
            return R::EMPTY;
        
        out  = cdb::fragment(utf8(f));
        if(out)
            return R::OKAY;
        
        bool    ok  = false;
        uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
        if(!ok)
            return R::PARSE;
        
        out      = cdb::fragment(ut);
        return out ? R::OKAY : R::NOT_FOUND;
    }

    R  decode_fragment(const char* sz)
    {
        return decode_fragment(x_fragment, sz);
    }

    R  decode_fragment_prime()
    {
        x_fragment  = Fragment{};
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz_id);
        if(!f.isEmpty()){
            bool    ok  = false;
            uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
            if(ok){
                x_fragment    = cdb::fragment(ut);
                if(!x_fragment)
                    return R::NOT_FOUND;
                return R::OKAY;
            } else
                return R::PARSE;
        } 
        
        f       = x_request -> getParameter(sz_key);
        if(!f.isEmpty()){
            x_fragment    = cdb::fragment(utf8(f));
            if(!x_fragment)
                return R::NOT_FOUND;
            return R::OKAY;
        }
        
        return decode_fragment();
    }


    R  decode_graph(Graph&out, const char* sz)
    {
        out    = Graph();
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz);
        if(f.isEmpty())
            return R::EMPTY;
            
        bool    ok  = false;
        uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
        if(!ok)
            return R::PARSE;
        
        out = cdb::graph(ut);
        return out ? R::OKAY : R::NOT_FOUND;
    }

    R  decode_graph(const char* sz)
    {
        return decode_graph(x_graph, sz);
    }

    R  decode_graph_prime()
    {
        x_graph    = Graph();
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz_id);
        if(!f.isEmpty()){
            bool    ok  = false;
            uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
            if(ok){
                x_graph    = cdb::graph(ut);
                if(!x_graph)
                    return R::NOT_FOUND;
                return R::OKAY;
            } else
                return R::PARSE;
        } 
        
        
        return decode_graph();
    }

    R     decode_icon(Image&out, const char* sz)
    {
        out = Image();
        if(!x_request)
            return R::INTERNAL;
        
        QByteArray  f   = x_request -> getParameter(sz);
        if(f.isEmpty())
            return R::EMPTY;
        
        bool    ok  = false;
        uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
        if(!ok)
            return R::NOT_FOUND;
        
        out = cdb::image(ut);
        return out ? R::OKAY : R::NOT_FOUND;
    }

    R     decode_icon(const char* sz)
    {
        return decode_icon(x_icon, sz);
    }

    R     decode_icon_prime()
    {
        x_icon = Image();
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz_id);
        if(!f.isEmpty()){
            bool    ok  = false;
            uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
            if(ok){
                x_icon    = cdb::image(ut);
                if(!x_icon)
                    return R::NOT_FOUND;
                return R::OKAY;
            } else
                return R::PARSE;
        } 
        return decode_icon();
    }


    R     decode_key()
    {
        x_key.clear();
        if(!x_request)
            return R::INTERNAL;
        QByteArray  f   = x_request -> getParameter(sz_key).trimmed();
        if(f.isEmpty())
            return R::EMPTY;
        if(!valid_key(f))
            return R::PARSE;
        x_key           = utf8(f);
        return R::OKAY;
    }

    R   decode_leaf_string(Leaf& out, const std::string_view& s)
    {
        out     = cdb::leaf(s);
        if(out)
            return R::OKAY;
        auto id = s.to_uint64();
        if(!id.good)
            return R::NOT_FOUND;
        out = cdb::leaf(id.value);
        return out ? R::OKAY : R::NOT_FOUND;
    }

    R   decode_leaf_prime_string(const std::string_view&s)
    {
        return decode_leaf_string(x_leaf, s);
    }

    R  decode_leaf(Leaf& out, const char* sz)
    {
        out      = Leaf();
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz);
        if(f.isEmpty())
            return R::EMPTY;
        
        out   = cdb::leaf(utf8(f));
        if(out)
            return R::OKAY;
        
        
        bool    ok  = false;
        uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
        if(!ok)
            return R::NOT_FOUND;
        
        out = cdb::leaf(ut);
        return out ? R::OKAY : R::NOT_FOUND;
    }

    R  decode_leaf(const char* sz)
    {
        return decode_leaf(x_leaf, sz);
    }

    R  decode_leaf_prime()
    {
        x_leaf      = Leaf();
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz_id);
        if(!f.isEmpty()){
            bool    ok  = false;
            uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
            if(ok){
                x_leaf    = cdb::leaf(ut);
                if(!x_leaf)
                    return R::NOT_FOUND;
                return R::OKAY;
            } else
                return R::PARSE;
        } 
        
        f       = x_request -> getParameter(sz_key);
        if(!f.isEmpty()){
            x_leaf    = cdb::leaf(QString::fromUtf8(f));
            if(!x_leaf)
                return R::NOT_FOUND;
            return R::OKAY;
        }
        
        return decode_leaf();
    }

    R  decode_root(const Root*&out, const char* sz)
    {
        out  = nullptr;
        if(!x_request)
            return R::INTERNAL;
        QByteArray  f   = x_request -> getParameter(sz);
        if(f.isEmpty())
            return R::EMPTY;

        out          = Root::by_key(utf8(f).toStdString());
        if(out)
            return R::OKAY;
            
        bool    ok  = false;
        uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
        if(!ok)
            return R::NOT_FOUND;

        out      = wksp::root(ut);
        if(out)
            return R::OKAY;
        return R::NOT_FOUND;
    }

    R  decode_root(const char* sz)
    {
        return decode_root(x_root, sz);
    }

    R  decode_root_prime()
    {
        x_root  = nullptr;
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz_id);
        if(!f.isEmpty()){
            bool    ok  = false;
            uint64_t    ut  = (uint64_t) f.toULongLong(&ok);
            if(!ok)
                return R::PARSE;
            x_root  = wksp::root(ut);
            if(!x_root)
                return R::NOT_FOUND;
            return R::OKAY;
        } 
        
        return decode_root();
    }

    R  decode_tag(Tag& out, const char* sz)
    {
        out       = Tag{};
        if(!x_request)
            return R::INTERNAL;
        QByteArray  f   = x_request -> getParameter(sz_tag);
        if(f.isEmpty())
            return R::EMPTY;
            
        out       = cdb::tag(utf8(f));
        if(!out)
            return R::NOT_FOUND;
        return R::OKAY;
    }

    R  decode_tag(const char* sz)
    {
        return decode_tag(x_tag, sz);
    }

    R  decode_tag_prime()
    {
        x_tag       = Tag{};
        if(!x_request)
            return R::INTERNAL;
            
        QByteArray  f   = x_request -> getParameter(sz_key);
        if(!f.isEmpty()){
            x_tag    = cdb::tag(utf8(f));
            if(!x_tag)
                return R::NOT_FOUND;
            return R::OKAY;
        }
        
        return decode_tag();
    }


    bool        test(R r, bool fIncEmpty)
    {
        switch(r){
        case R::OKAY:
            return true;
        case R::EMPTY:
            if(!fIncEmpty)
                throw HttpStatus::BadRequest;
            return false;
        case R::PARSE:
            throw HttpStatus::BadRequest;
        case R::INTERNAL:
            throw HttpStatus::InternalError;
        case R::NOT_FOUND:
            throw HttpStatus::NotFound;
        }
        return true;
    }


}
