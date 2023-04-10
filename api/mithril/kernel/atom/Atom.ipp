////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Atom.hpp"
#include "AtomCDB.hpp"

#include <io/FileUtils.hpp>
#include <basic/TextUtils.hpp>
#include <meta/Init.hpp>
#include <mithril/kernel/atom/Class.hpp>
#include <mithril/kernel/db/NKI.hpp>
#include <mithril/kernel/file/Document.hpp>
#include <mithril/kernel/file/DocumentCDB.hpp>
#include <mithril/kernel/image/Image.hpp>
#include <mithril/kernel/org/Tag.hpp>
#include <mithril/kernel/wksp/Workspace.hpp>

namespace yq::mithril {
    std::vector<Atom>   Atom::all(Sorted sorted)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::all_atoms(sorted);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    std::vector<Atom>   Atom::all(Class cls, Sorted sorted)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::all_atoms(cls, sorted);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    std::vector<Atom>   Atom::all(Document doc, Sorted sorted)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::all_atoms(doc, sorted);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    std::vector<Atom>   Atom::all(Tag tag, Sorted sorted)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::all_atoms(tag, sorted);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    size_t              Atom::count()
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::count_atoms();
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return 0ULL;
        }
    }

    size_t              Atom::count(Class cls)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::count_atoms(cls);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return 0ULL;
        }
    }

    size_t              Atom::count(Document doc)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::count_atoms(doc);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return 0ULL;
        }
    }

    size_t              Atom::count(Tag tag)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::count_atoms(tag);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return 0ULL;
        }
    }
    
    //Atom                Atom::create(Document)
    //{
        //static const auto& oracle   = wksp::oracle();
        //switch(oracle.type){
        //case Oracle::DB:
            //break;
        //case Oracle::CURL:  // TODO
        //case Oracle::NONE:
        //default:
            //break;
        //}
    //}

    bool                Atom::exists(uint64_t i)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::exists_atom(i);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return false;
        }
    }

    Atom                Atom::find(uint64_t i)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::find_atom(i);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    Atom                Atom::find(std::string_view k)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::find_atom(k);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    Atom                Atom::find(Document doc)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::find_atom(doc);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    Atom                Atom::find(Document doc, std::string_view ck)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::find_atom(doc, ck);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    std::string         Atom::full_key(Document doc, std::string_view ck)
    {
        std::string     kk  = cdb::key(doc);
        std::string     dk = std::string(strip_extension(kk));
        if(dk.empty())    
            return std::string();

        if(!ck.empty()){
            dk += '#';
            dk += ck;
        }
        return dk;
    }
    
    std::vector<Atom>   Atom::named(std::string_view n, Sorted sorted)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::named_atoms(n, sorted);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    std::string         Atom::abbreviation() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::abbreviation(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    std::string         Atom::brief() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::brief(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    std::vector<Atom>   Atom::children(Sorted sorted) const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::children(*this, sorted);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    size_t              Atom::children_count() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::children_count(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return 0ULL;
        }
    }
    
    std::vector<Class>  Atom::classes(Sorted sorted) const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::classes(*this, sorted);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    size_t              Atom::classes_count() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::classes_count(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return 0ULL;
        }
    }
    
    Document            Atom::document() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::document(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    Image               Atom::icon() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::icon(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    Atom::Info          Atom::info() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::info(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    bool                Atom::is(Class cls) const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::is(*this, cls);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return false;
        }
    }
    
    bool                Atom::is(Tag tag) const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::is(*this, tag);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return false;
        }
    }
    
    bool                Atom::is_edge() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::is_edge(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return false;
        }
    }
    
    bool                Atom::is_leaf() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::is_leaf(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return false;
        }
    }

    std::string         Atom::key() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::key(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    std::string         Atom::label() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::label(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    Leaf                Atom::leaf() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::leaf(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    std::string         Atom::name() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::name(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    NKI                 Atom::nki(bool autoKeyToName) const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::nki(*this, autoKeyToName);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    Atom                Atom::parent() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::parent(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    std::string         Atom::skey() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::skey(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    std::vector<Tag>    Atom::tags(Sorted sorted) const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::tags(*this, sorted);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    size_t              Atom::tags_count() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::tags_count(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return 0ULL;
        }
    }
    
    std::string         Atom::title() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::title(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    bool                Atom::valid() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::valid(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return false;
        }
    }
}

