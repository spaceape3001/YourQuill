////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/cdb_common.hpp>
#include <db/atomsys/class.hpp>
#include <db/atomsys/field-file.hpp>
#include <db/imagesys/image.hpp>
#include <db/orgsys/category.hpp>

namespace yq {
    struct Field;
    struct TypeInfo;

    struct Field::Info {
        Class           class_;
        Image           icon;
        Category        category;
        std::string     brief;
        std::string     key, pkey;
        std::string     name, plural;
        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        using FieldFragDoc      = std::pair<Fragment, Field::SharedFile>;
        
        string_set_t            aliases(Field);

        std::vector<Field>           all_fields(Sorted sorted=Sorted());
        size_t                  all_fields_count();
        

        std::string             brief(Field);
        
        Category                category(Field);
        

        Class                   class_(Field);


        /*! \brief ALL classes this field can be in
        */
        std::vector<Class>       classes(Field, Sorted sorted=Sorted());
        
        /*! \brief All data types this field can use
        */
        string_set_t            data_types(Field);
        
        //Field                   db_field(Class c, std::string_view k, bool *wasCreated=nullptr);
        Field                   db_field(Document, bool *wasCreated=nullptr);
        
        //Field                   db_field(std::string_view , bool *wasCreated=nullptr);
        //Field                   db_field(Document, bool *wasCreated=nullptr);
        
        std::vector<Class>           def_classes(Field, Sorted sorted=Sorted());
        
        Document                document(Field);

        bool                    exists(Field);
        bool                    exists_field(uint64_t);

        Field                   field(uint64_t);
        Field                   field(Document);
        Field                   field(std::string_view);
        Field::SharedFile       field_doc(Fragment, unsigned int opts=0);

        Image                   icon(Field);

        Field::Info             info(Field, bool autoKeyToName=false);

        bool                    is_any(Field);

        std::string             key(Field);

        std::string             label(Field);
        
        //Leaf                    leaf(Atom

        Field::SharedData       merged(Field, unsigned int opts=0);
        //Class                   make_class(std::string_view , const Root* rt=nullptr);

        std::string             name(Field);
        
        NKI                     nki(Field, bool autoKeyToName=false);
        
        Class                   parent(Field);
        std::string             pkey(Field);    //!< Key for plurals
        
        std::string             plural(Field);
        
        //! \brief Returns the FIRST class fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Field::SharedFile       read(Field, const Root*, unsigned int opts=0);

        std::vector<FieldFragDoc>    reads(Field, unsigned int opts=0);
        std::vector<FieldFragDoc>    reads(Field, class Root*, unsigned int opts=0);

        std::vector<Tag>             tags(Field, Sorted sorted=Sorted());
        size_t                  tags_count(Field);

        bool                    tagged(Field, Tag);

        Field::SharedData       update_info(Field, unsigned int opts=0);


        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Field::SharedFile       writable(Field, const Root*, unsigned int opts=0);
    }
}
