////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Global.hpp"

#include "MetaField.hpp"
#include "Meta.hpp"
#include "MetaInternal.hxx"
#include "MetaObject.hpp"
#include "MetaProperty.hpp"
#include "MetaType.hpp"
#include "MetaValue.hpp"
#include "MetaValueImpl.hxx"
#include "Object.hpp"
#include "Variant.hpp"

#include "util/Map.hpp"
#include "util/String.hpp"
#include "util/Utilities.hpp"
#include "util/Vector.hpp"

#include <atomic>
#include <cassert>

#include <tbb/spin_mutex.h>

    /*
        Variant and Meta Type are combined nto the same FILE due to some underhanded dealings.
    */

unsigned int thread_id() 
{
    static std::atomic<unsigned int>                lastThread{0};
    static const thread_local unsigned int  tid =   lastThread++;
    return tid;
}


struct EmptyMetaValue : public MetaValue {
    EmptyMetaValue(const char* zName, unsigned int id) : MetaValue(zName, __FILE__, id) 
    {
        m_copyB     = [](DataBlock&, const DataBlock&){};
        m_ctorCopyR = [](DataBlock&, const void*) {};
        m_ctorCopyB = [](DataBlock&, const DataBlock&) {};
        m_ctorMove  = [](DataBlock&, DataBlock&&) {};
        m_dtor      = [](DataBlock&) {};
        m_equal     = [](const DataBlock&, const DataBlock&) -> bool { return true; };
        m_moveB     = [](DataBlock&, DataBlock&&) {};
        if(id)
            m_print = [](const void*) -> String { return "(wildcard)"; };
        else
            m_print = [](const void*) -> String { return "(empty)"; };
            
        insert();
    }
};


namespace {
    static constexpr const unsigned int         kReserve        = 8192;
    

    template <typename T>
    void    set_and_reserve(Vector<T>& vec, size_t idx, size_t inc, const T& val, const T& def)
    {
        if(vec.size() <= idx){
            if(vec.capacity() <= idx)
                vec.reserve(idx + inc);
            vec.resize(idx+1, def);
        }
        vec[idx]    = val;
    }

    struct Repo {
        volatile bool                           openReg;
        tbb::spin_mutex                         mutex;
        
        Vector<const Meta*>                     all;
        Vector<const Meta*>                     metas;
        
        Vector<const Global*>                   globals;
        Map<String,const Global*,IgCase>        global;

        Vector<const MetaObject*>               objs;
        Map<String,const MetaObject*,IgCase>    obj;

        Vector<const MetaType*>                 types;
        Map<String,const MetaType*,IgCase>      type;

        Vector<const MetaValue*>                values;
        Map<String,const MetaValue*,IgCase>     value;
        
        Hash<int, const MetaValue*>             qtTypes;
        
        Vector<void(*)()>                       freezers;

        Repo() : openReg(true)
        {
            all.reserve(kReserve);
            all.resize(MV_USER, nullptr);
            metas.reserve(kReserve);
            globals.reserve(kReserve);
            objs.reserve(kReserve);
            types.reserve(kReserve);
            values.reserve(kReserve);
            thread_id();
        }
        
        ~Repo() = delete;
    };
    
    Repo&     repo() 
    {
        static Repo*  s_repo = new Repo;
        return *s_repo;
    }
    
    inline bool is_closed() { return !repo().openReg; }
    inline bool is_open() { return repo().openReg; }


    const MetaValue&    mvInvalid()
    {
        static EmptyMetaValue*  s_ret = new EmptyMetaValue("void", MV_Invalid);
        return *s_ret;
    }
    
    const MetaValue&    mvVariant()
    {
        static EmptyMetaValue*  s_ret = new EmptyMetaValue("variant", MV_Variant);
        return *s_ret;
    }
    
    #define LOCK                                                                                \
        Repo&       _r      = repo();                                                           \
        tbb::spin_mutex::scoped_lock     _lock;                                                 \
        if(_r.openReg)                                                                          \
            _lock.acquire(_r.mutex);
}


void    meta_init()
{
    mvInvalid();
    mvVariant();
    DelayInit::init_all();
}

/*! \brief Registers a hook to be called during the final freeze process */
void    meta_freeze_hook(void(*f)())
{
    LOCK
    _r.freezers << f;
}

void    meta_freeze()
{
    //  TODO Add freezers..... (and switch to atomic integer in lieu of openReg .... )

    meta_init();
    LOCK
    _r.openReg      = false;
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//      GLOBALS
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



Vector<const Global*>    Global::all()
{
    return repo().globals;
}

Vector<String>           Global::keys()
{
    return repo().global.keys();
}

const Global*           Global::lookup(unsigned int i)
{
    const Meta* mi  = Meta::lookup(i);
    if(mi && mi -> is_global())
        return static_cast<const Global*>(mi);
    return nullptr;
}

const Global*            Global::lookup(const String&k)
{
    return repo().global.get(k,nullptr);
}

Variant                  Global::value(const String&k)
{
    const Global*   g   = lookup(k);
    return g ? g -> value() : Variant();
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


Global::Global(const char*k, const MetaValue&t) : Meta(k), m_type(t)
{
    Meta::m_type |= isGlobal;
    LOCK
    if(_r.openReg){
        _r.globals << this;
        _r.global[name()]       = this;
    }
}


Global::~Global()
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//      META 
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

const Vector<const Meta*>&  Meta::all()
{
    return repo().metas;
}

const Meta*                  Meta::lookup(unsigned int i)
{
    return repo().all.value(i, nullptr);
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Meta::Meta(const String& zName, unsigned int i) : m_label(zName), m_name(zName), m_id(iBAD), m_type(0ULL)
{
    LOCK
    if(_r.openReg){
        if(i < MV_USER){
            m_id    = i;
            _r.all[i]   = this;
        } else {
            m_id    = _r.all.size();
            _r.all << this;
        }
        if(i >= MV_String)
            _r.metas << this;
    }
}

Meta::~Meta()
{
    assert(false && "Do NOT delete Meta!  It's not meant to be deleted.");
}

void        Meta::alias(const String&z)
{
    LOCK
    if(!_r.openReg) 
        return ;

    m_aliases << z;
}

void        Meta::description(const String& z)
{
    LOCK
    if(!_r.openReg)
        return ;

    m_description       = z;
}

bool        Meta::has_tag(const String&k) const
{
    return m_tags.has(k);
}

void        Meta::label(const String&z)
{
    if(is_closed())
        return ;
    m_label     = z;
}

void        Meta::name(const String&z)
{
    LOCK
    if(!_r.openReg)
        return ;

    m_name          = z;
}

const Variant&          Meta::tag(const String&k) const
{
    static const Variant  s_invalid;
    auto f = m_tags.find(k);
    if(f != m_tags.end())
        return *(f->second);
    return s_invalid;
}

void        Meta::tag(const String&k, const Variant&v)
{
    Variant *dup    = new Variant(v);
    
    LOCK
    if(!_r.openReg){
        delete dup;
        return ;
    }
    
    /*
        We can't do anything with an existing (old) value because it may have been handed out.
        So, yes, this will constitute a slight memory leak .... don't retag!
    */
    
    m_tags[k]       = dup;
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//      META FIELD
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MetaField::MetaField(const char* k, MetaValue& o, const MetaValue&t) : Meta(k), m_parent(o), m_value(t)
{   
    Meta::m_type       |= isField;
    o.m_type           |= isStruct;
    o.m_field[k]        = this;
    o.m_fields << k;
}

MetaField::Writer&     MetaField::Writer::description(const String&v)
{
    if(m_field)
        m_field -> description(v);
    return *this;
}

MetaField::Writer&     MetaField::Writer::label(const String&v)
{
    if(m_field)
        m_field -> label(v);
    return *this;
}

MetaField::Writer&     MetaField::Writer::tag(const String&k, const Variant&v)
{
    if(m_field)
        m_field -> tag(k,v);
    return *this;
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//      META OBJECT
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

const Vector<const MetaObject*>&    MetaObject::all()
{
    return repo().objs;
}

const MetaObject*            MetaObject::lookup(unsigned int i)
{
    const Meta* mi = Meta::lookup(i);
    if(mi && mi -> is_object())
        return static_cast<const MetaObject*>(mi);
    return nullptr;
}

const MetaObject*            MetaObject::lookup(const String&k)
{
    return repo().obj.get(k,nullptr);
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MetaObject::MetaObject(const char* z, const char* f) : MetaType(z, f, iBAD), m_base(nullptr)
{
    Meta::m_type |= isObject;

    LOCK
    if(_r.openReg)
        _r.objs << this;
}

bool                MetaObject::can_cast_to(const MetaObject*mo) const
{
    return mo && ((mo == this) || is_base(mo));
}


void                MetaObject::inherit()
{
    if(m_base){
        m_base -> m_local.derives << this;
        m_all.prop      = m_base -> m_all.prop;
        m_all.props     = m_base -> m_all.props;
        m_all.method    = m_base -> m_all.method;
        m_all.methods   = m_base -> m_all.methods;
    }
    for(MetaObject* mo = m_base; mo; mo=mo->m_base){
        m_bases << mo;
        mo -> m_all.derives << this;
    }
}

void                MetaObject::insert() 
{
    MetaType::insert();
    m_all.prop      |= m_local.prop;
    m_all.props     += m_local.props;
    m_all.method    |= m_local.method;
    m_all.methods   += m_local.methods;
    
    LOCK
    if(_r.openReg){
        _r.objs << this;
        _r.obj[name()]  = this;
        _r.obj[label()]  = this;
        for(const String&a : aliases())
            _r.obj[a]    = this;
    }
}

bool                MetaObject::is_base(const MetaObject*mo) const
{
    for(const MetaObject* b : m_bases)
        if(b == mo)
            return true;
    return false;
}

bool                MetaObject::is_derived(const MetaObject*mo) const
{
    for(const MetaObject* d : m_all.derives)
        if(d == mo)
            return true;
    return false;
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//      META PROPERTY
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


MetaProperty::MetaProperty(const char*k, MetaObject&mo, const MetaValue&mv) : Meta(k), m_parent(mo), m_value(mv)
{
    Meta::m_type |= isProperty;
    mo.m_local.prop[k]   = this;
    mo.m_local.props << k;
}

MetaProperty::~MetaProperty()
{
}

Variant             MetaProperty::get(const Object&obj) const
{
    if(obj.meta().can_cast_to(&m_parent))
        return get_impl(obj);
    return Variant();
}

bool                MetaProperty::set(Object&obj, const Variant&v) const
{
    if(!obj.meta().can_cast_to(&m_parent))
        return false;
    if(!is_settable())
        return false;
    if(v.type() == m_value)
        return set_impl(obj, v.ptr());
    
    Variant v2  = v.convert_to(m_value);
    if(!v2.is_valid())
        return false;
    return set_impl(obj, v2.ptr());
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//      META TYPE
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

const Vector<const MetaType*>&  MetaType::all()
{
    return repo().types;
}

const MetaType*          MetaType::lookup(unsigned int i)
{
    const Meta* mi  = Meta::lookup(i);
    if(mi && mi -> is_type())
        return static_cast<const MetaType*>(mi);
    return nullptr;
}

const MetaType*          MetaType::lookup(const String&s)
{
    return repo().type.get(s, nullptr);
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


MetaType::MetaType(const char*z, const char* f, unsigned int i) : Meta(z, i), m_file(f)
{
    m_type |= isType;
    
    LOCK
    if(_r.openReg){
        if(i >= MV_String)
            _r.types   << this;
    }
}

    //MetaType(const char*, unsigned int);
MetaType::~MetaType()
{
}

void    MetaType::insert()
{
    LOCK
    if(_r.openReg){
        _r.type[name()]   = this;
        _r.type[label()]  = this;
        for(const String&a : aliases())
            _r.type[a]    = this;
    }
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//      META VALUE
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


const Vector<const MetaValue*>&  MetaValue::all()
{
    return repo().values;
}

const MetaValue&  MetaValue::invalid()
{
    return mvInvalid();
}

const MetaValue*  MetaValue::lookup(unsigned int i)
{
    const Meta* mi  = Meta::lookup(i);
    if(mi && mi -> is_value())
        return static_cast<const MetaValue*>(mi);
    return nullptr;
}

const MetaValue*  MetaValue::lookup(const String& z)
{
    return repo().value.get(z, nullptr);
}

const MetaValue&  MetaValue::variant()
{
    return mvVariant();
}


//  --------------------------------------------------------------------------------------------------------------------
//  --------------------------------------------------------------------------------------------------------------------

MetaValue::MetaValue(const char*zName, const char* f, unsigned int i) : MetaType(zName, f, i),
    m_size(0),
    m_qtType(0),
    m_small(false),
    m_copyB(nullptr),
    m_ctorCopyR(nullptr),
    m_ctorCopyB(nullptr),
    m_ctorMove(nullptr),
    m_dtor(nullptr),
    m_equal(nullptr),
    m_moveB(nullptr),
    m_print(nullptr),
    m_ioFormat(nullptr),
    m_ioParse(nullptr),
    m_toQVariant(nullptr),
    m_fromQVariant(nullptr)
{   
    Meta::m_type |= isValue;
    LOCK
    if(_r.openReg){ 
        if(i >= MV_String)
            _r.values << this;
    }
}


MetaValue::~MetaValue()
{
}

//void                    MetaValue::alias(const String&z) 
//{
    //Meta::alias(z);
    
    //LOCK
    //if(!_r.openReg)
        //return;
    //_r.type[z]    = this;
//}

MetaValue::FNConvert   MetaValue::converter(const MetaValue& mt) const
{
    return m_convert.get(&mt, nullptr);
}

const MetaField*        MetaValue::field(const String&k) const
{
    return m_field.get(k,nullptr);
}


Vector<const MetaField*>    MetaValue::fields() const
{
    return m_field.values();
}

bool        MetaValue::has_conversion(const MetaValue&mt) const
{
    return m_convert.has(&mt);
}

bool        MetaValue::has_field(const String&k) const
{
    return m_fields.has(k);
}

void        MetaValue::insert()
{
    MetaType::insert();
    LOCK
    if(_r.openReg){
        _r.value[name()]   = this;
        _r.value[label()]  = this;
        for(const String&a : aliases())
            _r.value[a]    = this;
    }
}

//bool        MetaValue::is_collection() const
//{
    //LOCK
    //return !m_keys.empty();
//}

//const MetaKey*              MetaValue::key(size_t i) const
//{
    //LOCK
    //return m_keys.value(i,nullptr);
//}


//Vector<const MetaKey*>      MetaValue::keys() const
//{
    //LOCK
    //return m_keys;
//}

void    MetaValue::qtType(int i)
{
    LOCK
    if(_r.openReg && !m_qtType){
        m_qtType        = i;
        _r.qtTypes[i]   = this;
    }
}




