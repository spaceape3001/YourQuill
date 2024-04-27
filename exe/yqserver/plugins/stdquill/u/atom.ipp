////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/CollectionUtils.hpp>

#include <mithril/atom/Atom.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/atom/AtomDiff.hpp>

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
    
    void    u_atom(Atom a, std::span<Attribute::Diff> attrs, Change chg, const UAtomParam& uPar={})
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
            CacheQuery("DELETE FROM AtomClass WHERE atom=?")
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
