////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Enum.hpp"
#include "Set.hpp"
#include "XmlFwd.hpp"

#include "rapidxml.hpp"
#include <type_traits>

boolean_r           x_boolean(const XmlBase*);
double_r            x_double(const XmlBase*);
float_r             x_float(const XmlBase*);
unsigned_r          x_hex(const XmlBase*);
uint8_r             x_hex8(const XmlBase*);
uint16_r            x_hex16(const XmlBase*);
uint32_r            x_hex32(const XmlBase*);
uint64_r            x_hex64(const XmlBase*);
int_r               x_int(const XmlBase*);
int_r               x_integer(const XmlBase*);
int8_r              x_int8(const XmlBase*);
int16_r             x_int16(const XmlBase*);
int32_r             x_int32(const XmlBase*);
int64_r             x_int64(const XmlBase*);
short_r             x_short(const XmlBase*);
std::string_view    x_string(const XmlBase*);
unsigned_r          x_uint(const XmlBase*);
uint8_r             x_uint8(const XmlBase*);
uint16_r            x_uint16(const XmlBase*);
uint32_r            x_uint32(const XmlBase*);
uint64_r            x_uint64(const XmlBase*);
unsigned_r          x_uinteger(const XmlBase*);
unsigned_r          x_unsigned(const XmlBase*);
ushort_r            x_ushort(const XmlBase*);

template <typename E>
Result<E>           x_enum(const XmlBase*xb)
{
    if(xb -> value_size() == 0)
        return Result<E>(E::default_value());
    auto   v    = E::value_for(x_string(xb));
    if(v.good)
        return Result<E>(v.value);
    return Result<E>();
}

template <typename Pred>
auto                read_attribute(const XmlNode* xn, const char* pszAttr, Pred pred)
{
    using Res = std::invoke_result_t<Pred, const XmlAttribute*>;
    const XmlAttribute*xa   = xn -> first_attribute(pszAttr);
    if(xa)
        return pred(xa);
    return Res();
}

template <typename Pred>
auto                read_child(const XmlNode* xn, const char* pszTag, Pred pred)
{
    using Res = std::invoke_result_t<Pred, const XmlNode*>;
    const XmlNode*      xb  = xn -> first_node(pszTag);
    if(xb)
        return pred(xb);
    return Res();
}

template <typename Pred>
auto                read_children(const XmlNode* xn, const char* pszTag, Pred pred)
{
    using Res = std::invoke_result_t<Pred, const XmlNode*>;
    Vector<Res> ret;
    for(const XmlNode* xb = xn->first_node(pszTag); xb; xb = xb -> next_sibling(pszTag))
        ret << pred(xb);
    return ret;
}




/*
    With the exception of bool, the rest use the xml document for allocation purposes
*/

XmlDocument*         document_for(XmlBase* xb);

void                 write_hex(XmlBase* xb, uint8_t);
void                 write_hex(XmlBase* xb, uint16_t);
void                 write_hex(XmlBase* xb, uint32_t);
void                 write_hex(XmlBase* xb, uint64_t);

void                 write_x(XmlBase* xb, bool);
void                 write_x(XmlBase* xb, double);
void                 write_x(XmlBase* xb, float);

void                 write_x(XmlBase* xb, int8_t);
void                 write_x(XmlBase* xb, int16_t);
void                 write_x(XmlBase* xb, int32_t);
void                 write_x(XmlBase* xb, int64_t);

void                 write_x(XmlBase* xb, uint8_t);
void                 write_x(XmlBase* xb, uint16_t);
void                 write_x(XmlBase* xb, uint32_t);
void                 write_x(XmlBase* xb, uint64_t);

void                 write_x(XmlBase* xb, const std::string&);
void                 write_x(XmlBase* xb, const String&);

template <typename E>
void                 write_x(XmlBase* xb, EnumImpl<E> v)
{
    write_x(xb, v.key());
}

template <typename T>
void                write_xn(XmlNode*xn, const T& val) 
{
    write_x(xn, val);
}



template <typename T>
void                write_attribute(XmlNode* xb, const char* pszAttr, const T& value, bool copyAttr=false)
{
    XmlDocument*    doc = xb -> document();
    XmlAttribute*   xa  = xb -> document() -> allocate_attribute();
    if(copyAttr){
        xa -> name( doc -> allocate_string(pszAttr));
    } else
        xa -> name(pszAttr);
    xb -> append_attribute(xa);
    write_x(xa, value);
}

template <typename T>
XmlNode*             write_child(XmlNode* xb, const char* pszTag, const T& value, bool copyTag=false)
{
    XmlDocument*    doc = xb -> document();
    XmlNode*        xn  = xb -> document() -> allocate_node(rapidxml::node_element);
    if(copyTag){
        xn -> name(doc->allocate_string(pszTag));
    } else 
        xn -> name(pszTag);
    xb->append_node(xn);
    write_xn(xn, value);
    return xn;
}

template <typename T>
XmlNode*             write_child(XmlNode* xb, const String& pszTag, const T& value)
{
    return write_child(xb, pszTag.c_str(), value, true);
}

template <typename T,typename C>
void    write_children(XmlNode* xb, const char* pszTag, const Set<T,C>& values)
{
    XmlDocument*    doc = xb -> document();
    for(const T& v : values){
        XmlNode*    xn  = doc -> allocate_node(rapidxml::node_element, pszTag);
        xb -> append_node(xn);
        write_xn(xn, v);
    }
}

template <typename T>
void    write_children(XmlNode* xb, const char* pszTag, const Vector<T>& values)
{
    XmlDocument*    doc = xb -> document();
    for(const T& v : values){
        XmlNode*    xn  = doc -> allocate_node(rapidxml::node_element, pszTag);
        xb -> append_node(xn);
        write_xn(xn, v);
    }
}

