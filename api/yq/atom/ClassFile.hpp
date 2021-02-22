////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "yq/io/StdFile.hpp"
#include "yq/util/Set.hpp"

//! Atom classes (including fields)
class ClassFile : public StdFile {
public:

    struct AttrInfo;
    struct ValueInfo;

    static constexpr Seq zAtom      = { "atom" };
    static constexpr Seq zAttr      = { "attr" };
    static constexpr Seq zAttrName  = { "name", "%" };
    static constexpr Seq zBrief     = { "desc", "brief", "%desc", "%brief" };
    static constexpr Seq zName      = { "%", "%name", "name" };
    static constexpr Seq zNote      = { "note", "note", "notes", "%notes" };
    static constexpr Seq zPlural    = { "plural", "%plural"};
    static constexpr Seq zSource    = { "src", "%src", "source" };
    static constexpr Seq zTarget    = { "tgt", "%tgt", "target" };
    static constexpr Seq zType      = { "type", "%type" };
    static constexpr Seq zUse       = { "use", "%use", "is", "%is",  "atom", "%atom", "class","%class" };
    static constexpr Seq zValue     = { "value"  };

    Vector<AttrInfo>    attrs() const;

    String              brief() const;
    void                brief(const String&);
    
    bool                edge() const;
    
    String              name() const;
    void                name(const String&);

    String              notes() const;
    void                notes(const String&);
    
    String              plural() const;
    void                plural(const String&);
    
    StringSet           source() const;
    void                source(const StringSet&);

    StringSet           target() const;
    void                target(const StringSet&);
    
    StringSet           use() const;
    void                use(const StringSet&);

private:
    virtual bool        recursive_attributes() const override { return true; }
    virtual bool        has_body() const override { return false; }
};

struct ClassFile::AttrInfo {
    String              key;
    String              plural;
    String              name;
    String              desc;
    StringSet           types;
    StringSet           atoms;
    
    AttrInfo(){}
    explicit AttrInfo(const Attribute*);
};

struct ClassFile::ValueInfo {
    String              value;
    String              desc;
    String              type;
    
    ValueInfo() {}
    explicit ValueInfo(const Attribute*);
};


