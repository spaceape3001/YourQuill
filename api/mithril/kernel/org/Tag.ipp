////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Tag.hpp"
#include "TagCDB.hpp"

#include <io/FileUtils.hpp>
#include <basic/TextUtils.hpp>
#include <meta/Init.hpp>
#include <mithril/kernel/atom/AtomCDB.hpp>
#include <mithril/kernel/db/NKI.hpp>
#include <mithril/kernel/file/Document.hpp>
#include <mithril/kernel/file/DocumentCDB.hpp>
#include <mithril/kernel/image/Image.hpp>
#include <mithril/kernel/wksp/Workspace.hpp>


namespace yq {
    std::vector<Tag>        Tag::all(Sorted sorted)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::all_tags(sorted);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    std::vector<Tag>        Tag::all(Atom a, Sorted sorted)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::tags(a, sorted);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    std::vector<Tag>        Tag::all(const string_set_t&v)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::find_tags(v);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    std::vector<Tag>        Tag::all(const string_view_set_t&v)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::find_tags(v);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    size_t                  Tag::count()
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::count_tags();
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    size_t                  Tag::count(Atom a)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::tags_count(a);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    bool                    Tag::exists(uint64_t i)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::exists_tag(i);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    Tag                     Tag::find(uint64_t i)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::find_tag(i);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    Tag                     Tag::find(std::string_view k)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::find_tag(k);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    Tag                     Tag::find(Document doc)
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::find_tag(doc);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    std::string             Tag::brief() const
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

    Document                Tag::document() const
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

    Image                   Tag::icon() const
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

    Tag::Info               Tag::info(bool autoKeyToName) const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::info(*this, autoKeyToName);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }

    std::string             Tag::key() const
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

    std::string             Tag::label() const
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

    Leaf                    Tag::leaf() const
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

    std::string             Tag::name() const
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

    NKI                     Tag::nki(bool autoKeyToName) const
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

    bool                    Tag::valid() const
    {
        static const auto& oracle   = wksp::oracle();
        switch(oracle.type){
        case Oracle::DB:
            return cdb::valid(*this);
        case Oracle::CURL:  // TODO
        case Oracle::NONE:
        default:
            return {};
        }
    }
}
