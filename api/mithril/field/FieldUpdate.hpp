////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>
#include <mithril/field/FieldData.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/id/ById.hpp>
#include <mithril/update/U.hpp>

namespace yq::mithril::update {

    using TypeInfoSet   = std::set<const TypeInfo*, ById>;

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
        TypeInfoSet         dataTypes;
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
        TypeInfoSet enum_dataTypes() const;
    };
}
