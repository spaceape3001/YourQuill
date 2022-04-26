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
        
        string_set_t            aliases(Class);

        /*! \brief All classes in the cache database
        
            \param[in]  sorted  Yes/No to sort by key (default is no)
            \return vector of classes
        */
        Vector<Class>           all_classes(Sorted sorted=Sorted{});
        
        /*! \brief Count of all classes declared in the cache
        */
        size_t                  all_classes_count();

        std::string             brief(Class);

        Class                   class_(uint64_t);
        Class                   class_(std::string_view );

        Class::SharedFile       class_doc(Fragment, unsigned int opts=0);
        
        Vector<Class>           classes(const string_set_t&);
        
        Class                   db_class(Document, bool *wasCreated=nullptr);
        Class                   db_class(std::string_view, bool *wasCreated=nullptr);
        
        Vector<Class>           def_derived(Class, Sorted sorted=Sorted());
        Vector<Class>           def_reverse(Class, Sorted sorted=Sorted());
        Vector<Class>           def_source(Class, Sorted sorted=Sorted());
        Vector<Class>           def_target(Class, Sorted sorted=Sorted());
        Vector<Class>           def_use(Class, Sorted sorted=Sorted());
        
        //Graph                   dep_graph(Class);

        Vector<Class>           dependents(Class, Sorted sorted=Sorted());
        
        //Document                document(Atom);
        Document                document(Class);

        
        Vector<Class>           edges_in(Class, Sorted sorted=Sorted());
        Vector<Class>           edges_out(Class, Sorted sorted=Sorted());
      
        bool                    exists(Class);

        bool                    exists_class(uint64_t);
        Field                   field(Class, std::string_view);
        Vector<Field>           fields(Class, Sorted sorted=Sorted());
        size_t                  fields_count(Class);

        Image                   icon(Class);
        
        //Vector<Atom>            inbound(Atom);
        Vector<Class>            inbound(Class, Sorted sorted=Sorted());

        Class::Info             info(Class, bool autoKeyToName=false);

        bool                    is(Class classInQuestion, Class base);
        bool                    is_all(Class, std::initializer_list<Class>);
        bool                    is_any(Class, std::initializer_list<Class>);

        //bool                    is_edge(Class);
        //bool                    is_tagged(Class, Tag);

        std::string             key(Class);
        std::string             label(Class);

        Class::SharedData       merged(Class, unsigned int opts=0);
        Class                   make_class(std::string_view , const Root* rt=nullptr);

        std::string             name(Class);
        
        NKI                     nki(Class, bool autoKeyToName=false);
        
        //Vector<Atom>            outbound(Atom);
        Vector<Class>           outbound(Class, Sorted sorted=Sorted());
        
        std::string             plural(Class);
        

        //! \brief Returns the FIRST class fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Class::SharedFile       read(Class, const Root*, unsigned int opts=0);

        Vector<ClassFragDoc>    reads(Class, unsigned int opts=0);
        Vector<ClassFragDoc>    reads(Class, class Root*, unsigned int opts=0);

        Vector<Class>            reverses(Class, Sorted sorted=Sorted());
        
        Vector<Class>           sources(Class, Sorted sorted=Sorted());
        size_t                  sources_count(Class);
        
        Vector<Class>           targets(Class, Sorted sorted=Sorted());
        size_t                  targets_count(Class);
        
        Vector<Tag>             tags(Class, Sorted sorted=Sorted());
        size_t                  tags_count(Class);


        Vector<Class>           uses(Class, Sorted sorted=Sorted());
        size_t                  uses_count(Class);


        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Class::SharedFile       writable(Class, const Root*, unsigned int opts=0);
    }
}
