#pragma once

#include "AbstractFile.hpp"
#include "yq/bit/Attribute.hpp"

class Stream;


class StdFile : public AbstractFile, public AttrTree {
public:

    String              body;
    
    bool                empty() const;

protected:
    virtual void        reset() override;
    virtual bool        read(Vector<char>&buffer, const std::string& fname) override;
    virtual bool        write(Vector<char>&) override;
    
    void                write_out(Stream&, const Vector<Attribute>&, unsigned int mxkey, unsigned int depth=0) const;
    
private:
    virtual bool        recursive_attributes() const { return true; }
    virtual bool        has_body() const { return false; }
    static void         add_attr(Vector<Attribute>&, size_t&, Attribute&);
    
};

class StdBodyFile : public StdFile {
public:

private:
    virtual bool        has_body() const { return true; }
};


