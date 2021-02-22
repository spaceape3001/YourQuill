////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "yq/bit/Context.hpp"
#include "yq/enum/Format.hpp"
#include "yq/io/StdFile.hpp"

//! Leaf, the basis of all evil :)
class LeafFile : public StdFile {
public:
    using Shared    = std::shared_ptr<LeafFile>;

    static constexpr Seq zAbbr      = { "%abbr" };
    static constexpr Seq zBrief     = { "%desc", "%brief" };
    static constexpr Seq zNote      = { "%note", "%notes" };
    static constexpr Seq zTitle     = { "%", "%leaf", "%title" };
    static constexpr Seq zClass     = { "%is", "%cls", "%class", "%use", "%as", "%atom" };
    static constexpr Seq zTag       = { "%tag", "%tags" };
    static constexpr Seq zFormat    = { "%fmt", "%format" };
    static constexpr Format defFormat   = Format::Markdown;

    LeafFile(){}
    ~LeafFile(){}

    String      abbr() const;
    void        abbr(const String&);
    String      brief() const;
    void        brief(const String&);
    StringSet   classes() const;
    void        classes(const StringSet&);
    Context     context() const;
    Format      format() const;
    void        format(Format);
    String      notes() const;
    void        notes(const String&);
    StringSet   tags() const;
    void        tags(const StringSet&);
    String      title() const;
    void        title(const String&);

private:
    virtual bool        recursive_attributes() const override { return true; }
    virtual bool        has_body() const override { return true; }
};

