////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/cdb_common.hpp>
#include "file.hpp"
#include <db/class/struct.hpp>


namespace yq {
    struct Field;
    struct TypeInfo;

    struct Field::Info {
        Class            class_;
        std::string      brief;
        std::string      key, pkey;
        std::string      name, plural;
        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        
        string_set_t            aliases(Field);

        Vector<Field>           all_fields(Sorted sorted=Sorted());
        size_t                  all_fields_count();
        

        std::string             brief(Field);

        Class                   class_(Field);


        /*! \brief ALL classes this field can be in
        */
        Vector<Field>           classes(Field, Sorted sorted=Sorted());
        
        /*! \brief All data types this field can use
        */
        StringSet               data_types(Field);
        
        //Field                   db_field(Class c, std::string_view k, bool *wasCreated=nullptr);
        Field                   db_field(Document, bool *wasCreated=nullptr);
        
        //Field                   db_field(std::string_view , bool *wasCreated=nullptr);
        //Field                   db_field(Document, bool *wasCreated=nullptr);
        
        Vector<Class>           def_classes(Field, Sorted sorted=Sorted());
        
        Document                document(Field);

        bool                    exists(Field);
        bool                    exists_field(uint64_t);

        Field                   field(uint64_t);
        Field::SharedFile       field_doc(Fragment, bool fAllowEmpty=false);

        Image                   icon(Field);

        Field::Info             info(Field, bool autoKeyToName=false);

        bool                    is_any(Field);

        std::string             key(Field);

        std::string             label(Field);
        
        //Leaf                    leaf(Atom

        Field::SharedData       merged(Field, unsigned int opts=0);
        Class                   make_class(std::string_view , const Root* rt=nullptr);

        std::string                  name(Class);
        std::string                  name(Field);
        
        NKI                     nki(Class, bool autoKeyToName=false);
        NKI                     nki(Field, bool autoKeyToName=false);
        
        //Vector<Atom>            outbound(Atom);
        ClassVec                outbound(Class, Sorted sorted=Sorted());
        

        Class                   parent(Field);
        std::string                  pkey(Field);    //!< Key for plurals
        
        std::string                  plural(Class);
        std::string                  plural(Field);
        
        QList<QVariant>         qvar_list(const Set<Class>&);
        QList<QVariant>         qvar_list(const Set<Field>&);


        //! \brief Returns the FIRST class fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        ClassFile::Shared       read(Class, const Root*);

        Vector<Class::FragDoc>  reads(Class);
        Vector<Class::FragDoc>  reads(Class, class Root*);

        ClassVec                reverses(Class, Sorted sorted=Sorted());
        
        ClassVec                sources(Class, Sorted sorted=Sorted());
        size_t                  sources_count(Class);
        
        ClassVec                targets(Class, Sorted sorted=Sorted());
        size_t                  targets_count(Class);
        
        TagVec                  tags(Class, Sorted sorted=Sorted());
        TagVec                  tags(Field, Sorted sorted=Sorted());
        size_t                  tags_count(Class);
        size_t                  tags_count(Field);

        bool                    tagged(Class, Tag);
        bool                    tagged(Field, Tag);

        ClassVec                uses(Class, Sorted sorted=Sorted());
        size_t                  uses_count(Class);


        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        ClassFile::Shared       writable(Class, const Root*);


    }


}
