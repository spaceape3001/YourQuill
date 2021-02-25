////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "StdFile.hpp"
#include "Format.hpp"

class TagFile : public StdFile {
public:

    using Shared        = std::shared_ptr<TagFile>;

    //  TAGFILE *IS* Markdown

    static constexpr Seq zName      = { "%", "%tag", "tag", "%name", "name" };
    static constexpr Seq zNote      = { "%note", "note", "notes", "%notes" };
    static constexpr Seq zBrief     = { "%desc", "brief", "desc", "%brief" };
    static constexpr Seq zLeaf      = { "%leaf", "leaf" };
    static constexpr const Format kFormat  = Format::Markdown;

    String          brief() const;
    void            brief(const String&);
    String          leaf() const;
    void            leaf(const String&);
    String          notes() const;
    void            notes(const String&);

    String          name() const;
    void            name(const String&);
    

private:
    virtual bool        recursive_attributes() const override { return false; }
    virtual bool        has_body() const override { return false; }
};
