////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "MetaType.hpp"
#include "DataBlockImpl.hxx"
#include "yq/util/Hash.hpp"
#include "yq/util/String.hpp"

class Variant;
class VariantRef;
union DataBlock;
struct EmptyMetaValue;
template <typename, bool> class BaseMetaValueImpl;
template <typename T> class MetaValueImpl;

struct MetaField;
struct MetaKey;

class MetaValue : public MetaType {
    friend class Variant;
    template <typename> friend class MetaValueImpl;
    template <typename, bool> friend class BaseMetaValueImpl;
    friend struct EmptyMetaValue;
    friend struct VariantMetaValue;
public:

    template <typename T> struct Binder;

    static const Vector<const MetaValue*>&  all();
    static const MetaValue&                 invalid();
    static const MetaValue*                 lookup(unsigned int);
    static const MetaValue*                 lookup(const String&);
    static const MetaValue&                 variant();
    
    bool                            can_io_format() const { return m_ioFormat != nullptr; }
    bool                            can_io_parse() const { return m_ioParse != nullptr; }

    const MetaField*                field(const String&) const;
    const StringSet&                field_names() const { return m_fields; }
    Vector<const MetaField*>        fields() const;

    bool                            has_conversion(const MetaValue&) const;
    bool                            has_field(const String&) const;
    bool                            has_fields() const { return !m_fields.empty(); }

    //bool                            is_collection() const;
    
    //const MetaKey*                  key(size_t) const;
    
    //Vector<const MetaKey*>           keys() const;

    size_t                          size() const { return m_size; }
    bool                            small() const { return m_small; }

    virtual const char*             generic() const override  { return "MetaValue"; }
    
    bool                            operator==(const MetaValue& b) const { return this == &b; }

protected:
    ~MetaValue();
    MetaValue(const char*,const char*, unsigned int i);
    
    virtual void                    insert() override;
    
private:
    friend class MetaField;

    DataBlock                   m_defCopy;
    size_t                      m_size;
    bool                        m_small;
    //Vector<const MetaField*>    m_keys;
    
    
    using FieldMap  = Map<String,const MetaField*,IgCase>;
    FieldMap                m_field;
    StringSet               m_fields;

    MetaValue(const MetaValue&) = delete;
    MetaValue(MetaValue&&) = delete;
    void operator=(const MetaValue&) = delete;
    void operator=(MetaValue&&) = delete;
    
    typedef void        (*FNConvert)(void*, const void*);
    using ConvertSpec   = std::pair<const MetaValue*, FNConvert>;
    
    Hash<const MetaValue*, FNConvert>  m_convert;


    typedef void        (*FNCopyBlkBlk)(DataBlock&, const DataBlock&);
    FNCopyBlkBlk            m_copyB;

    //  COPY CONSTRUCTORS
    typedef void        (*FNCtorCopyRawBlk)(DataBlock&, const void*);
    FNCtorCopyRawBlk        m_ctorCopyR;
    
    typedef void        (*FNCtorCopyBlkBlk)(DataBlock&, const DataBlock&);
    FNCtorCopyBlkBlk        m_ctorCopyB;
    
    
    typedef void        (*FNCtorMove)(DataBlock&, DataBlock&&);
    FNCtorMove              m_ctorMove;
    

    //  DESTRUCTOR
    typedef void        (*FNDtor)(DataBlock&);
    FNDtor                  m_dtor;

    typedef bool        (*FNEqual)(const DataBlock&, const DataBlock&);
    FNEqual                 m_equal;


    typedef void        (*FNMoveBlkBlk)(DataBlock&, DataBlock&&);
    FNMoveBlkBlk            m_moveB;
    
    
    FNConvert           converter(const MetaValue&) const;
    
    //ConvertSpec             m_print;
    typedef String      (*FNPrint)(const void*);
    FNPrint                 m_print;

    
    typedef String      (*FNFormat)(const void*);
    typedef bool        (*FNParse)(const String&, void*);
    
    FNFormat                m_ioFormat;
    FNParse                 m_ioParse;
    
};


template <typename T>
struct MetaValue::Binder {
    enum { Defined = 0 };
    //static const MetaValue&              metaValue();
    //static MetaValueImpl<T>&             editValue();
};

template <>
struct MetaValue::Binder<void> {
    enum { Defined = 1 };
    static constexpr const char* szDefName   = "Void"; 
    static const MetaValue&              metaValue() { return invalid(); }
};

template <>
struct MetaValue::Binder<Variant> {
    enum { Defined = 1 };
    static constexpr const char* szDefName   = "Any"; 
    static const MetaValue&          metaValue() { return variant(); }
};

#define MV_DECLARE(name)                                                \
    template <>                                                         \
    struct MetaValue::Binder<name> {                                     \
        enum { Defined = 1 };                                           \
        static constexpr const char*    szDefName = #name;              \
        static const MetaValue&          metaValue();                     \
        static MetaValueImpl<name>&      editValue();                     \
    };

template <typename T>
const MetaValue&     metaValue()
{
    return MetaValue::Binder<T>::metaValue();
}

template <>
inline const MetaValue&    metaValue<void>()
{
    return MetaValue::invalid();
}


