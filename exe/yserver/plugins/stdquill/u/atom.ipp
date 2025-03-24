////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

//#include <0/basic/CollectionUtils.hpp>

#include <mithril/atom/Atom.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/atom/AtomDiff.hpp>
#include <mithril/field/FieldCDB.hpp>

#include <mithril/attribute/Attribute.hpp>

namespace {
    struct UAtomParam {
        std::string     key, name;
        Image           icon;
        bool            startup = false;
    };

    using SVSeq = std::initializer_list<std::string_view>;

    std::string     atom_brief(std::span<const Attribute::Diff> attrs, const UAtomParam& uPar)
    {
        static constexpr SVSeq  keys = { "%desc"sv, "%brief"sv };
    
        for(const Attribute::Diff& ad : attrs){
            if(is_in(ad.key, keys) && !ad.deleted())
                return ad.value;
        }
        
        return {};
    }

    ClassSet        atom_classes(std::span<const Attribute::Diff> attrs, const UAtomParam& uPar)
    {
        static constexpr SVSeq  keys = { "%is"sv, "%cls"sv, "%class"sv, "%use"sv, "%as"sv, "%atom"sv };
        
        string_set_t    useStr;
        for(const Attribute::Diff& ad : attrs){
            if(is_in(ad.key, keys) && !ad.deleted())
                useStr.insert(ad.value);
        }
        
        ClassSet    ret     = ret;
        for(Class c : cdb::db_classes(useStr)){
            ret += c;
            ret += cdb::base_classes(c);
        }
        return ret;
    }

    std::string     atom_name(std::span<const Attribute::Diff> attrs, const UAtomParam& uPar)
    {
        static constexpr SVSeq  keys = { "%"sv, "%name"sv, "%title"sv };
        if(!uPar.name.empty())
            return uPar.name;
        for(const Attribute::Diff& ad : attrs){
            if(is_in(ad.key, keys) && !ad.deleted())
                return ad.value;
        }
        return {};
    }
    
    TagSet          atom_tags(std::span<const Attribute::Diff> attrs, const UAtomParam& uPar)
    {
        static constexpr SVSeq  keys = { "%tag"sv, "%tags"sv };

        string_set_t    tagStr;
        for(const Attribute::Diff& ad : attrs){
            if(is_in(ad.key, keys) && !ad.deleted())
                tagStr.insert(ad.value);
        }

        return cdb::db_tags_set(tagStr);
    }
    
    void    u_atom(Atom a, std::span<Attribute::Diff> attrs, Change chg, const UAtomParam& uPar={});

    void    i_attribute_field(Atom a, Class c, Field f, Atom::Diff& x, const Attribute::Diff& ad)
    {
        //  It's a regular field.... maybe...
        static thread_local CacheQuery  iAttr("INSERT INTO AtomProperty (atom, attr, field, class) VALUES (?,?,?,?)");
        iAttr.bind(1, a.id);
        iAttr.bind(2, ad.attr.id);
        iAttr.bind(3, f.id);
        iAttr.bind(4, c.id);
        iAttr.exec();
        
        //  Tables....? (ie, more)
    }
    
    void    i_attribute_class(Atom a, Class c, Atom::Diff& x, const Attribute::Diff& ad)
    {
        std::string k(x.key);
        k += '/';
        k += to_string_view(ad.attr.id);

        Atom    ca  = cdb::db_atom(a, k);
        
        static thread_local CacheQuery  iAttr("INSERT INTO AtomProperty (atom, attr, class) VALUES (?,?,?,?)");
        iAttr.bind(1, a.id);
        iAttr.bind(2, ad.attr.id);
        iAttr.bind(3, ca.id);
        iAttr.exec();
        
        static thread_local CacheQuery  iClass("INSERT INTO AtomClass (atom,class) VALUES (?,?)");
        iClass.bind(1, a.id);
        iClass.bind(2, ca.id);
        iClass.exec();
        

        
        // TODO Update the sub....
        // u_atom(ca, x.sub, {});
    }

    void    i_attribute_plain(Atom a, Atom::Diff& x, const Attribute::Diff& ad)
    {
        //  It's a regular field.... maybe...
        static thread_local CacheQuery  iAttr("INSERT INTO AtomProperty (atom, attr) VALUES (?,?)");
        iAttr.bind(1, a.id);
        iAttr.bind(2, ad.attr.id);
        iAttr.exec();
    }

    void    i_attribute(Atom a, Atom::Diff& x, const Attribute::Diff& ad)
    {
        for(Class c : x.classes.to){
            Field f = cdb::field(c, x.key);
            if(f){
                i_attribute_field(a,c,f,x,ad);
                return;
            }
        }
        
        Class c = cdb::class_(x.key);
        if(c){
            i_attribute_class(a, c, x, ad);
            return ;
        }
        
        i_attribute_plain(a, x, ad);
    }
    
    void    u_attribute(Atom a, Atom::Diff& x, const Attribute::Diff& ad)
    {
    }

    void    x_attribute(Atom a, Atom::Diff& x, const Attribute::Diff& ad)
    {
    }
    
    void    u_atom(Atom a, std::span<Attribute::Diff> attrs, Change chg, const UAtomParam& uPar)
    {
        if(!a)
            return ;
            
        Atom::Diff      x { 
            .x = a, 
            .id = a.id, 
            .doc = cdb::document(a), 
            .chg=chg, 
            .key = cdb::key(a), 
            .diffs=attrs 
        };
        
        if(chg != Change::Added){
            x.classes.from      = cdb::classes_set(a);
            x.tags.from         = cdb::tags_set(a);
            x.name.from         = cdb::name(a);
            x.abbreviation.from = cdb::abbreviation(a);
        }
        
        static thread_local CacheQuery  uAtom("UPDATE Atoms SET name=?,brief=?,abbr=?,icon=? WHERE id=?");
        static thread_local CacheQuery  iClass("INSERT INTO AtomClass (atom, class) VALUES (?,?)");
        static thread_local CacheQuery  dClass("DELETE FROM AtomClass WHERE atom=? AND class=?");
        static thread_local CacheQuery  iTag("INSERT INTO AtomTag (atom, tag) VALUES (?,?)");
        static thread_local CacheQuery  dTag("DELETE FROM AtomTag WHERE atom=? AND tag=?");
        static thread_local CacheQuery  xAtom[] = {
            CacheQuery("DELETE FROM Atoms WHERE id=?"),
            CacheQuery("DELETE FROM AtomClass WHERE atom=?"),
            CacheQuery("DELETE FROM AtomProperty WHERE atom=?")
        };
        
        if(chg != Change::Removed){
            x.name.to           = atom_name(attrs, uPar);
            x.brief.to          = atom_brief(attrs, uPar);
            x.tags.to           = atom_tags(attrs, uPar);
            x.classes.to        = atom_classes(attrs, uPar);
            
            uAtom.bind(1, x.name.to);
            uAtom.bind(2, x.brief.to);
            uAtom.bind(3, x.abbreviation.to);
            uAtom.bind(4, x.icon.to.id);
            uAtom.bind(5, x.id);
            uAtom.exec();
            
            x.tags.analyze();            
            for(Tag j : x.tags.added)
                iTag.exec(x.id, j);
            
            x.classes.analyze();
            for(Class j : x.classes.added)
                iClass.exec(x.id, j);
                
            for(auto& ad : attrs){
                if(ad.key[0] == '%')    // skip system attributes
                    continue;
                if(ad.added()){
                    x.attributes.to.insert(ad.attr);
                    x.attributes.added.insert(ad.attr);
                    i_attribute(a,x,ad);
                } else if(ad.deleted()){
                    x.attributes.from.insert(ad.attr);
                    x.attributes.removed.insert(ad.attr);
                    x_attribute(a,x,ad);
                } else if(ad.modified()){
                    x.attributes.to.insert(ad.attr);
                    x.attributes.from.insert(ad.attr);
                    x.attributes.modified.insert(ad.attr);
                    u_attribute(a,x,ad);
                } else {
                    x.attributes.to.insert(ad.attr);
                    x.attributes.from.insert(ad.attr);
                    x.attributes.same.insert(ad.attr);
                }
            }    
        }
        
        Atom::Notify::notify(x);
        
        if(chg != Change::Removed){
            for(Tag j : x.tags.removed)
                dTag.exec(x.id, j);
            for(Class j : x.classes.removed)
                dClass.exec(x.id, j);
        }
        
        if(chg == Change::Removed){
            for(auto& sq : xAtom)
                sq.exec(x.id);
        }
    }

    void    s4_atom()
    {
        //  assimilate edges
    }
}
