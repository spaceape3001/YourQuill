#pragma once

#include "AbstractFile.hpp"
#include "yq/util/String.hpp"
#include "yq/util/Vector.hpp"

class Stream;


//! \brief Attribute
//! 
//! This is a generic attribute, expected to be a key/value nature.  It has several other features to allow for
//! reasonable data fusion.  It's nature is recursive, however, whether it is or not is up to the specific file.
struct Attribute {

    //! \brief Command
    //! This is the optional command for the attribute.  It's intended for add/remove/modify/comment/etc
    //! However, '%' is reserved for system defined attribute keys
    String              cmd;
    
    //! \brief Attribute Identifier
    //! This is the identifier for the attribute, it'll be surrounded by [] in the file and is expected to be
    //! alpha-numeric in nature.  Limited punctuation acceptable, spaces ignored on read
    String              id;
    
    //! \brief Attribute Key
    //! This is the key, and is expected to present in everything except a comment.  Keys are expected to be
    //! ALpha-numeric in nature.  Limited punctuation acceptable, spaces will trip the value
    String              key;
    
    //! \brief Attribute Value/data
    //! This is the attribute's valueData.  If the command isa comment, then this is the contents of that comment.
    String              data;
    
    //! \brief Sub-Attributes
    //! These are the sub-attributes (honored for recursive attributes)
    Vector<Attribute>   attrs;

    //! \brief Indent of the attribute
    //! Ignored on write, this is the original indentation value for the attribute from th efile.
    unsigned int        indent = 0;

    //! \brief Finds ALL attributes of the specified key
    //! \note This returns pointers for efficiency, however, these pointers can be INVALIDATED by a modification of the attribute vector!
    Vector<const Attribute*>    all(const String& key) const;
    
    Vector<const Attribute*>    all(const char*) const;
    
    //! \brief Finds ALL attributes of the specified key
    //! \note This returns pointers for efficiency, however, these pointers can be INVALIDATED by a modification of the attribute vector!
    Vector<const Attribute*>    all(const std::initializer_list<String>&) const;
    
    Vector<const Attribute*>    all(const std::initializer_list<const char*>&) const;
    
    //! \brief Finds ALL attributes of the specified key
    //! \note This returns pointers for efficiency, however, these pointers can be INVALIDATED by a modification of the attribute vector!
    Vector<const Attribute*>    all(const Vector<String>&) const;

    //! \brief Is empty?  
    //! This checks the strings/subattributes for emptyness and returns appropriately
    bool                        empty() const;
    
    //! \brief Erases ALL attributes with the speicfied key
    void                        erase_all(const String& key);
    void                        erase_all(const char* key);
    //! \brief Erases ALL attributes with the speicfied keys
    void                        erase_all(const std::initializer_list<String>&);
    void                        erase_all(const std::initializer_list<const char*>&);
    //! \brief Erases ALL attributes with the speicfied keys
    void                        erase_all(const Vector<String>&);
    
    //! \brief Erases ALL attributes after the first with the spedcified key
    void                        erase_seconds(const String& key);
    void                        erase_seconds(const char* key);

    //! \brief Erases ALL attributes after the first with the spedcified keys
    void                        erase_seconds(const std::initializer_list<String>&);
    void                        erase_seconds(const std::initializer_list<const char*>&);

    //! \brief Erases ALL attributes after the first with the spedcified keys
    void                        erase_seconds(const Vector<String>&);

    //! \brief Returns the FIRST attribute
    //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
    Attribute*                  first(const String& key);

    Attribute*                  first(const char* key);

    //! \brief Returns the FIRST attribute
    //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
    Attribute*                  first(const std::initializer_list<String>&);

    Attribute*                  first(const std::initializer_list<const char*>&);

    //! \brief Returns the FIRST attribute
    //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
    Attribute*                  first(const Vector<String>& keys);

    //! \brief Returns the FIRST attribute
    //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
    const Attribute*            first(const String& key) const;
    const Attribute*            first(const char* key) const;

    //! \brief Returns the FIRST attribute
    //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
    const Attribute*            first(const std::initializer_list<String>&) const;
    const Attribute*            first(const std::initializer_list<const char*>&) const;

    //! \brief Returns the FIRST attribute
    //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
    const Attribute*            first(const Vector<String>& keys) const;
    
    bool                        has(const String&) const;
    bool                        has(const char*) const;
    bool                        has(const std::initializer_list<String>&) const;
    bool                        has(const std::initializer_list<const char*>&) const;
    bool                        has(const Vector<String>&) const;
    
    //! \brief Returns the LAST attribute
    //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
    const Attribute*            last(const String& key) const;
    const Attribute*            last(const char* key) const;

    //! \brief Returns the LAST attribute
    //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
    const Attribute*            last(const std::initializer_list<String>&) const;
    const Attribute*            last(const std::initializer_list<const char*>&) const;

    //! \brief Returns the LAST attribute
    //! \note This returns a pointer for efficiency, however, this pointer can be INVALIDATED by a modification of the attribute vector!
    const Attribute*            last(const Vector<String>& keys) const;
    
    //! \brief Sets the specified attribute
    //!
    //!  This sets the first attribute encountered to the specified value,   If the attribute does not exist, then
    //!  it will be created.
    //!
    //! \note WARNING, this will (possibly) modify the vector, consider existing pointers to be invalid
    //!
    //! \param  key     Key to set and modify
    //! \param  value   New value
    //! \param  purge   If true, deletes all OTHER attirubtes with the same key
    //! \return Pointer to the attribute
    Attribute*                  set(const String&key, const String& value, bool purge=false);

    //! \brief Sets the specified attribute
    //!
    //!  This sets the first attribute encountered to the specified value,   If the attribute does not exist, then
    //!  it will be created.
    //!
    //! \note WARNING, this will (possibly) modify the vector, consider existing pointers to be invalid
    //!
    //! \param  key     Key to set and modify
    //! \param  value   New value
    //! \param  purge   If true, deletes all OTHER attirubtes with the same key
    //! \return Pointer to the attribute
    Attribute*                  set(const char* key, const String& value, bool purge=false);

    //! \brief Sets the specified attribute
    //!
    //!  This sets the first attribute encountered to the specified value,   If the attribute does not exist, then
    //!  it will be created using the FIRST key in the list.
    //!
    //! \note WARNING, this will (possibly) modify the vector, consider existing pointers to be invalid
    //!
    //! \param  key     Key to set and modify
    //! \param  value   New value
    //! \param  purge   If true, deletes all OTHER attirubtes with the same key
    //! \return Pointer to the attribute
    Attribute*                  set(const std::initializer_list<String>&key, const String& value, bool purge=false);

    //! \brief Sets the specified attribute
    //!
    //!  This sets the first attribute encountered to the specified value,   If the attribute does not exist, then
    //!  it will be created using the FIRST key in the list.
    //!
    //! \note WARNING, this will (possibly) modify the vector, consider existing pointers to be invalid
    //!
    //! \param  key     Key to set and modify
    //! \param  value   New value
    //! \param  purge   If true, deletes all OTHER attirubtes with the same key
    //! \return Pointer to the attribute
    Attribute*                  set(const std::initializer_list<const char*>&key, const String& value, bool purge=false);

    //! \brief Sets the specified attribute
    //!
    //!  This sets the first attribute encountered to the specified value,   If the attribute does not exist, then
    //!  it will be created using the FIRST key in the vector.
    //!
    //! \note WARNING, this will (possibly) modify the vector, consider existing pointers to be invalid
    //!
    //! \param  key     Key to set and modify
    //! \param  value   New value
    //! \param  purge   If true, deletes all OTHER attirubtes with the same key
    //! \return Pointer to the attribute
    Attribute*                  set(const Vector<String>&key, const String& value, bool purge=false);

    Attribute*                  set_set(const char*, const StringSet&, const String& sep=", ");
    Attribute*                  set_set(const String&, const StringSet&, const String& sep=", ");
    
    //! \brief Sets the specified attribute to a string set
    //! 
    //! \note WARNING, this will (possibly) modify the vector, consider existing pointers to be invalid
    //! \note This will ASSUME purge=true!
    Attribute*                  set_set(const std::initializer_list<const char*>&, const StringSet&, const String& sep=", ");

    Attribute*                  set_set(const std::initializer_list<String>&, const StringSet&, const String& sep=", ");
    Attribute*                  set_set(const Vector<String>&, const StringSet&, const String& sep=", ");
    
    
    //! \brief First value for string, empty if not found (or empty w/in)
    String                      value(const String& key) const;
    String                      value(const char* key) const;
    
    //! \brief First value for string, empty if not found (or empty w/in)
    String                      value(const std::initializer_list<String>&) const;
    String                      value(const std::initializer_list<const char*>&) const;

    //! \brief First value for string, empty if not found (or empty w/in)
    String                      value(const Vector<String>&) const;
    
    //! \brief All non-empty values for string
    Vector<String>              values(const String& key) const;
    Vector<String>              values(const char* key) const;

    //! \brief All non-empty values for string
    Vector<String>              values(const std::initializer_list<String>&) const;
    Vector<String>              values(const std::initializer_list<const char*>&) const;

    //! \brief All non-empty values for string
    Vector<String>              values(const Vector<String>&) const;
    
    StringSet                   values_set(const char*, const String& sep=",") const;
    StringSet                   values_set(const String&, const String& sep=",") const;
    StringSet                   values_set(const std::initializer_list<const char*>&, const String& sep=",") const;
    StringSet                   values_set(const std::initializer_list<String>&, const String& sep=",") const;
    StringSet                   values_set(const Vector<String>&, const String& sep=",") const;
};


void    fusion_attrs(Attribute&, const Attribute&);

class StdFile : public AbstractFile, public Attribute {
public:
    using Seq          = const std::initializer_list<const char*>;

    String              body;
    
    bool                empty() const;

protected:
    virtual void        reset() override;
    virtual bool        read(Vector<char>&buffer, const std::string& fname) override;
    virtual bool        write(Vector<char>&) override;
    
    void                write_out(Stream&, const Vector<Attribute>&, unsigned int depth=0) const;
    
private:
    virtual bool        recursive_attributes() const { return true; }
    virtual bool        has_body() const { return false; }
    static void         add_attr(Vector<Attribute>&, size_t&, Attribute&);
    
};
