////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/bit/Context.hpp>
#include <yq/enum/Format.hpp>
#include <yq/file/StdFile.hpp>

namespace yq {

    //! Leaf, the basis of all evil :)
    class LeafFile : public StdFile {
    public:
        using Shared    = std::shared_ptr<LeafFile>;

        static constexpr Seq zAbbr      = { "%abbr"sv };
        static constexpr Seq zBrief     = { "%desc"sv, "%brief"sv };
        static constexpr Seq zNote      = { "%note"sv, "%notes"sv };
        static constexpr Seq zTitle     = { "%"sv, "%leaf"sv, "%title"sv };
        static constexpr Seq zClass     = { "%is"sv, "%cls"sv, "%class"sv, "%use"sv, "%as"sv, "%atom"sv };
        static constexpr Seq zTag       = { "%tag"sv, "%tags"sv };
        static constexpr Seq zFormat    = { "%fmt"sv, "%format"sv };
        static constexpr Format defFormat   = Format::Markdown;

        LeafFile(){}
        ~LeafFile(){}

        std::string_view    abbr() const;
        void                abbr(const std::string_view&);
        std::string_view    brief() const;
        void                brief(const std::string_view&);
        string_view_set_t   classes() const;
        void                classes(const string_set_t&);
        void                classes(const string_view_set_t&);
        Context             context() const;
        Format              format() const;
        void                format(Format);
        std::string_view    notes() const;
        void                notes(const std::string_view&);
        string_view_set_t   tags() const;
        void                tags(const string_set_t&);
        void                tags(const string_view_set_t&);
        std::string_view    title() const;
        void                title(const std::string_view&);

    private:
        virtual bool        recursive_attributes() const override { return true; }
        virtual bool        has_body() const override { return true; }
    };

}
