////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/preamble.hpp>
#include <kernel/agw/Class.hpp>
#include <kernel/agw/FieldFile.hpp>
#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Sorted.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/org/Category.hpp>

namespace yq {
    struct TypeInfo;
    
    namespace agw {

        struct Field::Info {
            Class           class_;
            Image           icon;
            Category        category;
            std::string     brief;
            std::string     key, pkey;
            std::string     name, plural;
            bool operator==(const Info&) const = default;
        };

        struct Field::Rank {
            Field           field;
            uint64_t        rank = 0;
            constexpr auto    operator<=>(const Rank&rhs) const noexcept = default;
       };

        using FieldFragDoc      = std::pair<Fragment, Field::SharedFile>;
    }

    namespace cdb {
        
        string_set_t                    aliases(agw::Field);

        agw::FieldVector                all_fields(Sorted sorted=Sorted());
        size_t                          all_fields_count();
        

        std::string                     brief(agw::Field);
        
        Category                        category(agw::Field);
        

        agw::Class                      class_(agw::Field);


        /*! \brief ALL classes this field can be in
        */
        agw::ClassVector                classes(agw::Field, Sorted sorted=Sorted());
        
        /*! \brief All data types this field can use
        */
        std::set<uint64_t>              data_types(agw::Field);
        
        //agw::Field                   db_field(agw::Class c, std::string_view k, bool *wasCreated=nullptr);
        agw::Field                      db_field(Document, bool *wasCreated=nullptr);
        
        //agw::Field                   db_field(std::string_view , bool *wasCreated=nullptr);
        //agw::Field                   db_field(Document, bool *wasCreated=nullptr);
        
        agw::ClassVector                def_classes(agw::Field, Sorted sorted=Sorted());
        
        Document                        document(agw::Field);

        bool                            exists(agw::Field);
        bool                            exists_field(uint64_t);

        agw::Field                      field(uint64_t);
        
        /*! \brief agw::Field for document
        
            Returns the field for the specified document
            
            \param[in] calc Set to TRUE to use the document skeyc
        */
        agw::Field                      field(Document, bool calc=false);
        agw::Field                      field(std::string_view);
        agw::Field::SharedFile          field_doc(Fragment, cdb_options_t opts=0);

        Image                           icon(agw::Field);

        agw::Field::Info                info(agw::Field, bool autoKeyToName=false);

        bool                            is_any(agw::Field);
        bool                            is_tagged(agw::Field, Tag);

        std::string                     key(agw::Field);

        std::string                     label(agw::Field);
        
        //Leaf                    leaf(Atom
        
        agw::Field                      make_field(std::string_view, agw::Class, const Root* rt=nullptr, cdb_options_t opts=0, bool *wasCreated=nullptr);
        

        agw::Field::SharedData          merged(agw::Field, cdb_options_t opts=0);
        //agw::Class                   make_class(std::string_view , const Root* rt=nullptr);

        std::string                     name(agw::Field);
        
        NKI                             nki(agw::Field, bool autoKeyToName=false);
        
        agw::Class                      parent(agw::Field);
        std::string                     pkey(agw::Field);    //!< Key for plurals
        
        std::string                     plural(agw::Field);
        
        //! \brief Returns the FIRST class fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        agw::Field::SharedFile          read(agw::Field, const Root*, cdb_options_t opts=0);

        std::vector<agw::FieldFragDoc>  reads(agw::Field, cdb_options_t opts=0);
        std::vector<agw::FieldFragDoc>  reads(agw::Field, class Root*, cdb_options_t opts=0);

        std::vector<Tag>                tags(agw::Field, Sorted sorted=Sorted());
        size_t                          tags_count(agw::Field);
        std::set<Tag>                   tags_set(agw::Field);



        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        agw::Field::SharedFile          writable(agw::Field, const Root*, cdb_options_t opts=0);
    }
}
