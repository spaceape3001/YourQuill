////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassData.hpp"
#include "util/AbstractFile.hpp"

#include "StdFile.hpp"
#include "util/Set.hpp"

//! Atom classes (including fields)
class ClassFile : public AbstractFile, public ClassData {
public:

    using Shared    = std::shared_ptr<ClassFile>;

    //struct FieldInfo;
    //struct ValueInfo;


    //class FieldReader;
    //class FieldWriter;

    //static constexpr Seq zAtom      = { "atom" };
    //static constexpr Seq zAttr      = { "field", "attr" };
    //static constexpr Seq zAttrName  = { "name", "%" };
    //static constexpr Seq zBrief     = { "desc", "brief", "%desc", "%brief" };
    //static constexpr Seq zName      = { "%", "%name", "name" };
    //static constexpr Seq zNote      = { "note", "note", "notes", "%notes" };
    //static constexpr Seq zPlural    = { "plural", "%plural"};
    //static constexpr Seq zReverse   = { "reverse", "rev", "%rev" };
    //static constexpr Seq zSource    = { "source", "src", "%src"};
    //static constexpr Seq zTarget    = { "target", "tgt", "%tgt" };
    //static constexpr Seq zType      = { "type", "%type" };
    //static constexpr Seq zUse       = { "use", "%use", "is", "%is",  "atom", "%atom", "class","%class" };
    //static constexpr Seq zValue     = { "value"  };

    ////  Good for SHORT TERM USE ONLY!
    //FieldWriter         field(const String&, bool fCreate=false);
    ////  Good for SHORT TERM USE ONLY!
    //FieldReader         field(const String&) const;

    //Vector<FieldInfo>   fields() const;
    

    //String              brief() const;
    //void                brief(const String&);
    
    //bool                edge() const;
    
    //String              name() const;
    //void                name(const String&);

    //String              notes() const;
    //void                notes(const String&);
    
    //String              plural() const;
    //void                plural(const String&);
    
    //StringSet           reverse() const;
    //void                reverse(const StringSet&);
    
    //StringSet           source() const;
    //void                source(const StringSet&);

    //StringSet           target() const;
    //void                target(const StringSet&);
    
    //StringSet           use() const;
    //void                use(const StringSet&);

//private:
    //virtual bool        recursive_attributes() const override { return true; }
    //virtual bool        has_body() const override { return false; }

protected:
    virtual void    reset() override;
    virtual bool    read(Vector<char>&buffer, const std::string& fname) override;
    virtual bool    write(Vector<char>&) override;
};

#if 0

class ClassFile::FieldReader {
public:

    FieldReader() : m_ptr{}{}

    String          key() const;
    String          name() const;
    String          desc() const;
    String          plural() const;
    StringSet       types() const;
    StringSet       atoms() const;
    
    // TODO (Values)    

    operator bool() const { return static_cast<bool>(m_ptr); }

protected:
    friend class ClassFile;
    Attribute*      m_ptr;
    constexpr FieldReader(const Attribute*a) : m_ptr(const_cast<Attribute*>(a)){}
    constexpr FieldReader(Attribute* a) : m_ptr(a) {}

};

class ClassFile::FieldWriter : public FieldReader {
public:

    FieldWriter() : FieldReader{} {}

    using FieldReader::key;
    using FieldReader::name;
    using FieldReader::desc;
    using FieldReader::plural;
    using FieldReader::types;
    using FieldReader::atoms;
    
    void        name(const String&);
    void        desc(const String&);
    void        plural(const String&);
    void        types(const StringSet&);
    void        atoms(const StringSet&);
private:
    friend class ClassFile;
    constexpr FieldWriter(Attribute* a) : FieldReader(a) {}
};



struct ClassFile::FieldInfo {
    String              key;
    String              plural;
    String              name;
    String              desc;
    StringSet           types;
    StringSet           atoms;
    Vector<ValueInfo>   values;
    
    FieldInfo(){}
    explicit FieldInfo(const Attribute*);
};

struct ClassFile::ValueInfo {
    String              value;
    String              desc;
    String              type;
    
    ValueInfo() {}
    explicit ValueInfo(const Attribute*);
};


#endif
