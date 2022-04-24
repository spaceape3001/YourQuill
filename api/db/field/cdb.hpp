////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CacheFwd.hpp"
#include "ClassFile.hpp"
#include "FileSys.hpp"
#include "Graph.hpp"
#include "Image.hpp"


namespace yq {

    struct Field::Info {
        Class           class_;
        std::string      brief;
        std::string      key, pkey;
        std::string      name, plural;
        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        
        string_set_t            aliases(Field);

        FieldVec                all_fields(Sorted sorted=Sorted());
        size_t                  all_fields_count();
        
        bool                    any(Field);

        String                  brief(Field);

        Class                   class_(Field);

        Class::SharedFile       class_doc(Fragment, bool fAllowEmpty=false);
        
        ClassVec                classes(Field, Sorted sorted=Sorted());
        ClassVec                classes(const string_set_t&);
        
        
        string_set_t            data_types(Field);
            
        Class                   db_class(std::string_view , bool *wasCreated=nullptr);
        Class                   db_class(Document, bool *wasCreated=nullptr);
        ClassVec                db_classes(const string_set_t&);
        
        Field                   db_field(Class c, std::string_view k, bool *wasCreated=nullptr);
        Field                   db_field(std::string_view k, bool *wasCreated=nullptr);
        
        //Field                   db_field(std::string_view , bool *wasCreated=nullptr);
        //Field                   db_field(Document, bool *wasCreated=nullptr);
        
        ClassVec                def_derived(Class, Sorted sorted=Sorted());
        FieldVec                def_fields(Class, Sorted sorted=Sorted());
        ClassVec                def_reverse(Class, Sorted sorted=Sorted());
        ClassVec                def_source(Class, Sorted sorted=Sorted());
        ClassVec                def_target(Class, Sorted sorted=Sorted());
        ClassVec                def_use(Class, Sorted sorted=Sorted());
        
        Graph                   dep_graph(Class);
        Folder                  detail_folder(Class);

        Vector<Class>           dependents(Class, Sorted sorted=Sorted());
        
        //Document                document(Atom);
        Document                document(Class);
        Document                document(Field);

        bool                    edge(Class);
        
        ClassVec                edges_in(Class, Sorted sorted=Sorted());
        ClassVec                edges_out(Class, Sorted sorted=Sorted());
      
        bool                    exists(Class);
        bool                    exists(Field);

        bool                    exists_class(uint64_t);
        bool                    exists_field(uint64_t);

        Field                   field(uint64_t);
        Field                   field(Class, std::string_view );
        FieldVec                fields(Class, Sorted sorted=Sorted());
        size_t                  fields_count(Class);

        Image                   icon(Class);
        Image                   icon(Field);
        
        //Vector<Atom>            inbound(Atom);
        ClassVec                inbound(Class, Sorted sorted=Sorted());

        Class::Info             info(Class, bool autoKeyToName=false);
        Field::Info             info(Field, bool autoKeyToName=false);

        bool                    is(Class derived, Class base);
        //bool                    is_all(Class, std::initializer_list<Class>);
        //bool                    is_any(Class, std::initializer_list<Class>);

        String                  key(Class);
        String                  key(Field);

        String                  label(Class);
        String                  label(Field);
        
        //Leaf                    leaf(Atom

        ClassData::Shared       merged(Class, unsigned int opts=0);
        Class                   make_class(std::string_view , const Root* rt=nullptr);

        String                  name(Class);
        String                  name(Field);
        
        NKI                     nki(Class, bool autoKeyToName=false);
        NKI                     nki(Field, bool autoKeyToName=false);
        
        //Vector<Atom>            outbound(Atom);
        ClassVec                outbound(Class, Sorted sorted=Sorted());
        

        Class                   parent(Field);
        String                  pkey(Field);    //!< Key for plurals
        
        String                  plural(Class);
        String                  plural(Field);
        
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
