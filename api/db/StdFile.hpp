////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/bit/KeyValue.hpp>
#include <util/AbstractFile.hpp>

class Stream;


class StdFile : public AbstractFile, public KVTree {
public:

    String              body;
    
    bool                empty() const;

protected:
    virtual void        reset() override;
    virtual bool        read(Vector<char>&buffer, const std::string& fname) override;
    virtual bool        write(Vector<char>&) override;
    
private:
    virtual bool        recursive_attributes() const { return true; }
    virtual bool        has_body() const { return false; }
    
};

class StdBodyFile : public StdFile {
public:

private:
    virtual bool        has_body() const { return true; }
};


