////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/class/Class.hpp>
#include <mithril/enum/Multiplicity.hpp>
#include <mithril/enum/Restriction.hpp>
#include <mithril/field/FieldFile.hpp>
#include <mithril/image/Image.hpp>

#define TBL_FIELDS      "Fields"
#define TBL_FIELD_TAG   "FieldTag"

namespace yq::mithril {
    struct Field::Rank {
        Field           field;
        uint64_t        rank = 0;
        constexpr auto    operator<=>(const Rank&rhs) const noexcept = default;
   };
}

namespace yq::mithril::cdb {
    
    string_set_t            aliases(Field);

    std::vector<Field>      all_fields(Sorted sorted=Sorted());
    size_t                  all_fields_count();
    
    //ClassHopMap             atom_types(Field);

    std::string             brief(Field);
    
    Category                category(Field);
    

    Class                   class_(Field);


    /*! \brief ALL classes this field can be in
    */
    std::vector<Class>       classes(Field, Sorted sorted=Sorted());
    
    /*! \brief All data types this field can use
    */
    //std::set<uint64_t>      data_types(Field);
    
    //Field                   db_field(Class c, std::string_view k, bool *wasCreated=nullptr);
    Field                   db_field(Document, bool *wasCreated=nullptr);
    
    //Field                   db_field(std::string_view , bool *wasCreated=nullptr);
    //Field                   db_field(Document, bool *wasCreated=nullptr);
    
    //std::vector<Class>           def_classes(Field, Sorted sorted=Sorted());
    
    Document                document(Field);

    bool                    exists(Field);
    bool                    exists_field(uint64_t);
    
    Id                      expected(Field);

    Field                   field(uint64_t);
    
    /*! \brief Field for document
    
        Returns the field for the specified document
        
        \param[in] calc Set to TRUE to use the document skeyc
    */
    Field                   field(Document, bool calc=false);
    Field                   field(std::string_view);
    Field::SharedFile       field_doc(Fragment, cdb_options_t opts=0);

    Image                   icon(Field);

    Field::Info             info(Field, bool autoKeyToName=false);

    bool                    is_any(Field);

    std::string             key(Field);

    std::string             label(Field);
    
    //Leaf                    leaf(Atom
    
    Field                   make_field(std::string_view, Class, const RootDir* rt=nullptr, cdb_options_t opts=0, bool *wasCreated=nullptr);
    
    uint64_t                max_count(Field);

    Field::SharedData       merged(Field, cdb_options_t opts=0);
    //Class                   make_class(std::string_view , const RootDir* rt=nullptr);
    
    Multiplicity            multiplicity(Field);

    std::string             name(Field);
    
    NKI                     nki(Field, bool autoKeyToName=false);
    
    Class                   parent(Field);
    std::string             pkey(Field);    //!< Key for plurals
    
    std::string             plural(Field);
    
    //! \brief Returns the FIRST class fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    Field::SharedFile       read(Field, const RootDir*, cdb_options_t opts=0);

    std::vector<FieldFragDoc>   reads(Field, cdb_options_t opts=0);
    std::vector<FieldFragDoc>   reads(Field, class RootDir*, cdb_options_t opts=0);
    
    Restriction             restriction(Field);
    
    std::string             skey(Field);

    TagVector            tags(Field, Sorted sorted=Sorted());
    size_t                      tags_count(Field);
    TagSet               tags_set(Field);

    bool                    tagged(Field, Tag);
    
    std::vector<Id>         types(Field);


    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    Field::SharedFile       writable(Field, const RootDir*, cdb_options_t opts=0);
}
