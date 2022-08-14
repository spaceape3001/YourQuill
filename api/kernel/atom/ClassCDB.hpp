////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/atom/ClassFile.hpp>
#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Sorted.hpp>
#include <kernel/file/Document.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/org/Category.hpp>

//#include "Graph.hpp"


namespace yq {

    struct Fragment;
    struct Field;
    struct Tag;
    
    struct Class::Info {
        std::string key;
        Category    category;
        std::string binding;
        //Graph       deps;
        Document    doc;
        Image       icon;
        std::string name;
        std::string plural;
        std::string brief;
        bool        edge    = false;

        bool operator==(const Info&) const = default;
    };
    
    struct Class::Rank {
        Class       cls;
        int64_t     rank    = 0;
        constexpr auto    operator<=>(const Rank&rhs) const noexcept = default;
    };

    namespace cdb {
        using ClassFragDoc      = std::pair<Fragment, Class::SharedFile>;
        

        /*! \brief All classes in the cache database
        
            \param[in]  sorted  Yes/No to sort by key (default is no)
            \return vector of classes
        */
        std::vector<Class>          all_classes(Sorted sorted=Sorted{});
        
        /*! \brief Count of all classes declared in the cache
        */
        size_t                      all_classes_count();
        
        string_set_t                alternative_keys(Class);
        
        
        std::string                 binding(Class);

        std::string                 brief(Class);
        
        Category                    category(Class);

        Class                       class_(uint64_t);
        Class                       class_(Document, bool calc=false);
        Class                       class_(std::string_view );

        Class::SharedFile           class_doc(Fragment, cdb_options_t opts=0);
        
        std::vector<Class>          classes(const string_set_t&, bool noisy=false);
        std::vector<Class>          classes(Tag, Sorted sorted=Sorted{});
        std::set<Class>             classes_set(const string_set_t&, bool noisy=false);
        std::set<Class>             classes_set(const string_view_set_t&, bool noisy=false);
        
        Class                       db_class(Document, bool *wasCreated=nullptr);
        Class                       db_class(std::string_view, bool *wasCreated=nullptr);
        
        string_set_t                def_aliases(Class);
        std::vector<Class>          def_derived(Class, Sorted sorted=Sorted());
        std::vector<Field>          def_fields(Class c, Sorted sorted=Sorted{});
        std::vector<Class>          def_reverse(Class, Sorted sorted=Sorted());
        string_set_t                def_prefixes(Class);
        std::vector<Class>          def_source(Class, Sorted sorted=Sorted());
        string_set_t                def_suffixes(Class);
        std::vector<Class>          def_target(Class, Sorted sorted=Sorted());
        std::vector<Class>          def_use(Class, Sorted sorted=Sorted());
        
        //Graph                   dep_graph(Class);

        std::vector<Class>          dependents(Class, Sorted sorted=Sorted());

                //  Rank here is number of hops
        std::vector<Class::Rank>    dependents_ranked(Class);
        std::vector<Class::Rank>    dependents_ranked(Class, int64_t maxDepth);
        
        //Document                document(Atom);
        Document                    document(Class);

        
        std::vector<Class>          edges_in(Class, Sorted sorted=Sorted());
        std::vector<Class>          edges_out(Class, Sorted sorted=Sorted());
      
      
        //! Removes the class from the cache
        void                        erase(Class);
      
        bool                        exists(Class);

        bool                        exists_class(uint64_t);
        Field                       field(Class, std::string_view);
        std::vector<Field>          fields(Class, Sorted sorted=Sorted());
        size_t                      fields_count(Class);

        Image                       icon(Class);
        
        //std::vector<Atom>            inbound(Atom);
        std::vector<Class>          inbound(Class, Sorted sorted=Sorted());

        Class::Info                 info(Class, bool autoKeyToName=false);

        bool                        is(Class classInQuestion, Class base);
        bool                        is_all(Class, std::initializer_list<Class>);
        bool                        is_any(Class, std::initializer_list<Class>);

        //bool                    is_edge(Class);
        //bool                    is_tagged(Class, Tag);

        std::string                 key(Class);
        std::string                 label(Class);

        Class::SharedData           merged(Class, cdb_options_t opts=0);
        Class                       make_class(std::string_view, const Root* rt=nullptr, cdb_options_t opts=0, bool *wasCreated=nullptr);

        std::string                 name(Class);
        
        NKI                         nki(Class, bool autoKeyToName=false);
        
        //std::vector<Atom>            outbound(Atom);
        std::vector<Class>          outbound(Class, Sorted sorted=Sorted());
        
        std::string                 plural(Class);
        

        //! \brief Returns the FIRST class fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Class::SharedFile           read(Class, const Root*, cdb_options_t opts=0);

        std::vector<ClassFragDoc>   reads(Class, cdb_options_t opts=0);
        std::vector<ClassFragDoc>   reads(Class, class Root*, cdb_options_t opts=0);

        std::vector<Class>          reverses(Class, Sorted sorted=Sorted());
        
        std::vector<Class>          sources(Class, Sorted sorted=Sorted());
        size_t                      sources_count(Class);
        
        
        std::set<Tag>               tag_set(Class);
        std::vector<Tag>            tags(Class, Sorted sorted=Sorted());
        size_t                      tags_count(Class);

        std::vector<Class>          targets(Class, Sorted sorted=Sorted());
        size_t                      targets_count(Class);


        std::vector<Class>          uses(Class, Sorted sorted=Sorted());
        size_t                      uses_count(Class);
        std::vector<Class::Rank>    uses_ranked(Class);
        std::vector<Class::Rank>    uses_ranked(Class, int64_t depth);

        Class::SharedData           update(Class, cdb_options_t opts=0);
        void                        update_icon(Class);


        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Class::SharedFile           writable(Class, const Root*, cdb_options_t opts=0);
    }
}
