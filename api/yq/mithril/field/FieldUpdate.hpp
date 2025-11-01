////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/field/FieldData.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/tag/Tag.hpp>
#include <yq/mithril/id/ById.hpp>
#include <yq/mithril/update/U.hpp>

namespace yq::mithril::update {

    using TypeMetaSet   = std::set<const TypeMeta*, ById>;

    class UField : public U<Field> {
    public:
        static UField&      get(Field f);
        static std::pair<UField&, bool>  create(Document);
        
        static const FileSpec&  lookup();
        static void     notify(Fragment,Change);
        static void     icons(Fragment,Change);
        static void     s3(Document);

        
        const Document      doc;
        Field::SharedData   def;
        Image               icon;
        std::string         subkey;
        std::string         clskey;
        Class               cls;
        bool                anycls  = true;
        string_set_t        aliases;
        string_set_t        usurps;
        TypeMetaSet         dataTypes;
        TagSet              tags;
        ClassHopMap         atomTypes;

        UField(Field);
        
        void    reload();
        void    i_class();
        void    u_alias();
        void    u_atomTypes();
        void    u_dataTypes();
        void    u_info();
        void    u_icon();
        void    u_tags();
        void    u_usurp();
        void    x_erase();

        ClassHopMap enum_atomTypes() const;
        TypeMetaSet enum_dataTypes() const;
    };
}
