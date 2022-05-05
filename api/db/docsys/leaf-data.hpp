////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/docsys/leaf.hpp>
#include <db/bit/context.hpp>
#include <db/bit/key_value.hpp>
#include <yq/enum/format.hpp>

namespace yq {

    struct Leaf::Data {
        KVTree                  attrs;
        std::vector<Context>    context;
        
        void            reset();
        
        using Seq = std::initializer_list<std::string_view>;
        static constexpr Seq zAbbr      = { "%abbr"sv };
        static constexpr Seq zBrief     = { "%desc"sv, "%brief"sv };
        static constexpr Seq zClass     = { "%is"sv, "%cls"sv, "%class"sv, "%use"sv, "%as"sv, "%atom"sv };
        static constexpr Seq zFormat    = { "%fmt"sv, "%format"sv };
        static constexpr Seq zNote      = { "%note"sv, "%notes"sv };
        static constexpr Seq zTag       = { "%tag"sv, "%tags"sv };
        static constexpr Seq zTitle     = { "%"sv, "%leaf"sv, "%title"sv };
        static constexpr Seq zStart     = { "%start"sv, "start"sv };
        static constexpr Format defFormat   = Format::Markdown;

        Data&   merge(const Data&, bool fOverride=false);
        bool operator==(const Data&) const = default;

        std::string_view    abbr() const;
        void                abbr(const std::string_view&);
        std::string_view    brief() const;
        void                brief(const std::string_view&);
        string_view_set_t   classes() const;
        void                classes(const string_set_t&);
        void                classes(const string_view_set_t&);
        
        std::string_view    notes() const;
        void                notes(const std::string_view&);
        string_view_set_t   tags() const;
        void                tags(const string_set_t&);
        void                tags(const string_view_set_t&);
        std::string_view    title() const;
        void                title(const std::string_view&);
    };
    
    KeyValue        xn_key_value(const XmlNode*);
    void            read_xn(KeyValue&, const XmlNode*);
    void            write_xn(XmlNode*, const KeyValue&);
    
}
