////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/cdb_common.hpp>
#include "file.hpp"
#include <db/document/struct.hpp>
#include <db/enum/sorted.hpp>
#include <db/image/struct.hpp>

//#include "Graph.hpp"


namespace yq {

    struct Fragment;
    struct Field;
    
    struct Class::Info {
        std::string key;
        //Graph       deps;
        Document    doc;
        Image       icon;
        std::string name;
        std::string plural;
        std::string brief;
        bool        edge    = false;

        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        using ClassFragDoc      = std::pair<Fragment, Class::SharedFile>;
        
        StringSet               aliases(Class);

        /*! \brief All classes in the cache database
        
            \param[in]  sorted  Yes/No to sort by key (default is no)
            \return vector of classes
        */
        Vector<Class>          all_classes(Sorted sorted=Sorted{});
        
        /*! \brief Count of all classes declared in the cache
        */
        size_t                  all_classes_count();


        //FieldVec                all_fields(Sorted sorted=Sorted());
        //size_t                  all_fields_count();

        std::string             brief(Class);
        //String                  brief(Field);

        //Class                   class_(Field);
        Class                   class_(uint64_t);
        Class                   class_(std::string_view );

        Class::SharedFile       class_doc(Fragment, bool fAllowEmpty=false);
        
        Vector<Class>           classes(Field, Sorted sorted=Sorted());
        Vector<Class>           classes(const StringSet&);
        
        
        //StringSet               data_types(Field);
            
        Class                   db_class(Document, bool *wasCreated=nullptr);
        
        //Field                   db_field(Class c, std::string_view k, bool *wasCreated=nullptr);
        //Field                   db_field(std::string_view , bool *wasCreated=nullptr);
        //Field                   db_field(Document, bool *wasCreated=nullptr);
        
        Vector<Class>           def_derived(Class, Sorted sorted=Sorted());
        //FieldVec                def_fields(Class, Sorted sorted=Sorted());
        Vector<Class>           def_reverse(Class, Sorted sorted=Sorted());
        Vector<Class>           def_source(Class, Sorted sorted=Sorted());
        Vector<Class>           def_target(Class, Sorted sorted=Sorted());
        Vector<Class>           def_use(Class, Sorted sorted=Sorted());
        
        //Graph                   dep_graph(Class);
        //Folder                  detail_folder(Class);

        Vector<Class>           dependents(Class, Sorted sorted=Sorted());
        
        //Document                document(Atom);
        Document                document(Class);
        //Document                document(Field);

        
        Vector<Class>           edges_in(Class, Sorted sorted=Sorted());
        Vector<Class>           edges_out(Class, Sorted sorted=Sorted());
      
        bool                    exists(Class);
        //bool                    exists(Field);

        bool                    exists_class(uint64_t);
        //bool                    exists_field(uint64_t);

        //Field                   field(uint64_t);
        ////Field                   field(Class, std::string_view );
        Field                   field(Class, std::string_view);
        Vector<Field>           fields(Class, Sorted sorted=Sorted());
        size_t                  fields_count(Class);

        Image                   icon(Class);
        //Image                   icon(Field);
        
        //Vector<Atom>            inbound(Atom);
        Vector<Class>            inbound(Class, Sorted sorted=Sorted());

        Class::Info             info(Class, bool autoKeyToName=false);
        //Field::Info             info(Field, bool autoKeyToName=false);

        bool                    is_this(Class classInQuestion, Class base);
        //bool                    is_all(Class, std::initializer_list<Class>);
        //bool                    is_any(Class, std::initializer_list<Class>);

        bool                    is_edge(Class);

        std::string             key(Class);
        //String                  key(Field);

        std::string             label(Class);
        //String                  label(Field);
        
        //Leaf                    leaf(Atom

        Class::SharedData       merged(Class, unsigned int opts=0);
        Class                   make_class(std::string_view , const Root* rt=nullptr);

        std::string             name(Class);
        //String                  name(Field);
        
        NKI                     nki(Class, bool autoKeyToName=false);
        //NKI                     nki(Field, bool autoKeyToName=false);
        
        //Vector<Atom>            outbound(Atom);
        Vector<Class>          outbound(Class, Sorted sorted=Sorted());
        

        //Class                   parent(Field);
        //std::string             pkey(Field);    //!< Key for plurals
        
        std::string             plural(Class);
        //std::string             plural(Field);
        


        //! \brief Returns the FIRST class fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Class::SharedFile       read(Class, const Root*);

        Vector<ClassFragDoc>    reads(Class);
        Vector<ClassFragDoc>    reads(Class, class Root*);

        Vector<Class>            reverses(Class, Sorted sorted=Sorted());
        
        Vector<Class>           sources(Class, Sorted sorted=Sorted());
        size_t                  sources_count(Class);
        
        Vector<Class>           targets(Class, Sorted sorted=Sorted());
        size_t                  targets_count(Class);
        
        Vector<Tag>             tags(Class, Sorted sorted=Sorted());
        //TagVec                  tags(Field, Sorted sorted=Sorted());
        size_t                  tags_count(Class);
        //size_t                  tags_count(Field);

        bool                    is_tagged(Class, Tag);
        //bool                    tagged(Field, Tag);

        Vector<Class>           uses(Class, Sorted sorted=Sorted());
        size_t                  uses_count(Class);


        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Class::SharedFile       writable(Class, const Root*);
    }
}
