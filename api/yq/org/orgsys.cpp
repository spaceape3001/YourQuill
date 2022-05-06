////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CategoryData.hpp"
#include "CategoryFile.hpp"
#include "TagData.hpp"
#include "TagFile.hpp"

#include <yq/bit/KeyValue.hpp>
#include <yq/io/Strings.hpp>
#include <yq/text/text_utils.hpp>

#include <yq/db/IDLock.ipp>

namespace yq {

////////////////////////////////////////////////////////////////////////////////
//  CATEGORY
////////////////////////////////////////////////////////////////////////////////

    template class IDLock<Category>;

    Category::Data&   Category::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        return *this;
    }

////////////////////////////////////////////////////////////////////////////////

    void    Category::File::reset() 
    {
        *(Data*) this = Data{};
    }

    bool    Category::File::read(KVTree&&attrs, std::string_view fname) 
    { 
        name        = attrs.value(szName);
        brief       = attrs.value(szBrief);
        notes       = attrs.value(szNotes);
        return true; 
    }
        
    bool    Category::File::write(KVTree&attrs) const 
    { 
        if(!name.empty())
            attrs.set(szName, name);
        if(!brief.empty())
            attrs.set(szBrief, brief);
        if(!notes.empty())
            attrs.set(szNotes, notes);
        return true; 
    }

////////////////////////////////////////////////////////////////////////////////
//  TAG
////////////////////////////////////////////////////////////////////////////////

    template class IDLock<Tag>;

    Tag::Data& Tag::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(leaf, b.leaf, fOverride);
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        return *this;
    }

    void    Tag::Data::reset() 
    {
        name.clear();
        leaf.clear();
        brief.clear();
        notes.clear();
    }

////////////////////////////////////////////////////////////////////////////////


    void    Tag::File::reset() 
    {
        Data::reset();
    }

    #if TAG_XML_RESAVE
    bool    Tag::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQTag);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szTag);
        if(!xn){
            yError() << "No tag! In: " << fname << "'";
            return false;
        }
        
        name        = read_child(xn, szName, x_string);
        brief       = read_child(xn, szBrief, x_string);
        notes       = read_child(xn, szNotes, x_string);
        leaf        = read_child(xn, szLeaf, x_string);
        return true;
    }
    #endif

    bool    Tag::File::read(KVTree&&attrs, std::string_view fname) 
    {
        name    = attrs.value(kv::key({"%", "name"}));
        brief   = attrs.value("brief");
        notes   = attrs.value("notes");
        leaf    = attrs.value("leaf");
        return true;
    }
    
    bool    Tag::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("%", name);
        if(!brief.empty())
            attrs.set("brief", brief);
        if(!notes.empty())
            attrs.set("notes", notes);
        if(!leaf.empty())
            attrs.set("leaf", leaf);
        return true;
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
