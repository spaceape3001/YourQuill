#pragma once

#include "core/Enum.hpp"
#include "core/Flag.hpp"
#include "core/MetaValue.hpp"

class ClassDoc;
class LeafDoc;
class TagDoc;

/*! \brief Access amount 
*/
YQ_ENUM(Access, ,
    Default,
    NoAccess,
    ReadOnly,
    ReadWrite,
        //  if set, prefer to write in this location *first* on specified
        //  information
    WriteFirst
)

MV_DECLARE(Access)


/*! \enum AssertDeny
    \brief Assert/Deny something
    
    \var AssertDeny::Neither
    \brief No assertion/denial is being made, inherit it where possible
    
    \var AssertDeny::Deny
    \brief Explicity deny the thing
    
    \var AssertDeny::Assert
    \brief Explicity assert the thing
*/
YQ_ENUM(AssertDeny, , Neither, Deny, Assert)

/*! 
    \enum CalSystem
    \brief Calendar System
    
    \var CalSystem::Gregorian
    \brief  This is the Gregorian calendar
    
        \note This system will auto-populate the "fields" on run-time.

    \var CalSystem::Julian
    \brief  This is the Julian calendar
    
        \note This system will auto-populate the "fields" on run-time.

    \var CalSystem::DateZero
    \brief  New system based on Gregorian
    
        This is a calendar system that utilizes the Gregorian calendar, 
        in general, but assigns a custom date of "zero" and runs the years
        relative to that date.
        

        \note This system will auto-populate the "fields" on run-time.

    \var CalSystem::Custom
    \brief  You define it!
    
        Whatever calendar the user wants, they can specify the fields, including
        min/max values, relative sizes (if important)

*/
YQ_ENUM(CalSystem, ,
    Gregorian,
    Julian,
    DateZero,
    Custom
)


/*! \brief An area of policy

*/
YQ_ENUM(DataRole, , 
    //!  For all non-specific stuff
    Other,
    
    //! Web elements
    DB,

    //! CSS style
    Style,  
    
    //! Calendar Configuration
    CfgCal,
    
    Tags,
    
    Classes,
    
    //! Users
    Users,
    
    //! Images
    Image
)


YQ_ENUM(Format, , Markdown, ASCII, Plain, Html, Novel )


/*! 
    \enum Multiplicity
    \brief Multiplicity of something
    
    \var Restriction::Default
    \brief  Default multiplicity

    This value is meant to be "inherited" from others.
    \note Default of the default is all    

    \var Multilicity::Once 
    \brief Occurs only once
    
        When selected, this property may only appear once, and only once.
        (ie, date of birth).
    
    \var Multiplicity::Many
    \brief Multiple values
        
        When selected, this property can have multiple values.
    
    \var Multiplicity::Measurement
    
        When selected, this property will be time varying, the values
        are considered measurements.
    
    \var Multiplicity::All
        
        When selected, this property will can have multiple values at 
        any moment.  A value w/o date or time restraints will be valid
        throughout time.  
*/
YQ_ENUM(Multiplicity, , Default, Once, Many, Serial, All )

/*! 
    \enum Restriction
    \brief Restrictions on the values

    \var Restriction::Default
    \brief  Default restrictions
    
    This value is meant to be "inherited" from others.
    \note Default of the default is any    

    \var Restriction::Any
    \brief  No restrictions
    
    \var Restriction::Suggest
    \brief Suggest (by the user), but do not track
    
    \var Restriction::Track
    \brief Tracks the values in the fields, based on that.

    \var Restriction::Enumerate
    \brief Restricts the values to the specified, enumerated list
*/
YQ_ENUM(Restriction, , Default, Any, Suggest, Track, Enumerate)

YQ_ENUM(Vcs, , None, RCS, SVN, GIT)

using PolicyMap     = EnumMap<DataRole,Access>;

Result<Access>      decode_access(const String&arg);

/*! \brief Merges access levels

    This merges access levels, assuming a is superior
    
    \param[in]  Superior access
    \parma[in]  Subordinate access, modified
    \return Modified subordinate access, taking the superior into account
*/
Access      moderate(Access a, Access b);
Access      moderate(Access a, Access b, Access limit);

PolicyMap   moderate(const PolicyMap& a, const PolicyMap& b);
PolicyMap   moderate(const PolicyMap& a, const PolicyMap& b, Access limit);
PolicyMap   moderate(Access a, const PolicyMap& b);

//bool        xml_usable(const String&);

/*! \brief Writes a variant to the node
*/
void        write_xn(XmlNode*, const Variant&);

/*! \brief Gets a variant
*/
Variant     x_variant(const XmlNode*);

struct KVString {
    String      key, value;
};

void        write_xn(XmlNode*, const KVString&);
KVString    xn_kvstring(const XmlNode*);

StringSet       string_set(const Vector<std::string_view>& data, bool fParse=true);




