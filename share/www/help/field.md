#! Field

The field is an anticipated property of an [Atom](atom.md) with a specific (or generic) [Class](class.md).

## Files

All fields are defined in the fields folder (`/.fields`), and they have a corresponding `*.field` file extension.  This file follows the standard format, however, context is not currently supported.

### Key

The filename of the class definition, up to the final `.field` is taken as the field key.  This is case insensitive, and aside from one extra period, should be a valid [key](key.md).

A field key has an extra qualification.  *IF* it has a period, then it's expected to be `class.field` refinement, as explained in the next section.

### Class(es)

A field is expected to be applied to one or classes, or be globally applied to ALL classes.  If a `.` is present in the field key, then it's automatically applied to the class on the left, with the right bit being the expected property key.  Alternatively, the classes can be listed as class attributes within the file.  If neither of these are present, then the field will be applied to **ALL** classes.

### Field Name Attribute

    name    (Name of the field)

This attribute defines the proper name for the field, this can correct for capitalization, and/or characters not available on the filesystem.  Alternatively, it can be a full name, whereas it's easier to write a shorter name in all the files.

*SINGLE* use only, others will be ignored

### Alias Attribute

    alias   (alias)
    
Alternative key for the field, can be used anywhere.

*MULTIPLE* are permitted, all will be aggregated together, duplicates will be tolerated and ignored.

### Atom Attribute

    atom (class-key)
    
This denotes one acceptable interpretation of a user-value is an atom of the specified [classification](class.md).  Within the leaf/atom, the key/best-resolution lookup algorithm will be used to associate with the desired atom.

*MULTIPLE* are permitted, all will be aggregated together, duplicates will be tolerated and ignored.


### Brief Description Attribute

    brief   (Description)
    
This is a brief one-liner description for what this field is about.

*SINGLE* use only, others will be ignored

### Category Attribute

    category (category key)
    
This associates this field with the specified category for better organization.

*SINGLE* use only, others will be ignored


### Class Attribute

    class   (class-key)

This causes this field to be associated with the specified class.

*MULTIPLE* are permitted, all will be aggregated together, duplicates will be tolerated and ignored.


### Control Attribute

    control (value)
    
This controls the values for this property.  (Well, once implemented...)

* **Any** -- No restriction
* **Suggest** -- Some suggestions will be provided, but no data tracking will occur
* **Track** -- Values will be tracked in the cache, can fill the auto-suggest
* **Enumerate** -- Tightest control, restricts all entries to a prepopulated list of valid values.


### Data Type Attribute

    type    (typename)

This denotes an acceptable data type for an atom with this property.

*MULTIPLE* are permitted, all will be aggregated together, duplicates will be tolerated and ignored.

### Expected Attribute

    expect (typename)
    
This is the default type expected for a field, so it will error on this data type if uncertain.

### Maximum Count Attribute

    max (number)
    
This restricts the number of these types of properties permitted on an atom.  The default is zero which implies unrestricted.

*SINGLE* use only, others will be ignored

### Multiplicity Attribute

    allow (value)
    
This controls how to interpret (or even permit) multiple instances of the attribute.

* **Once** -- Only one instance is expected (equivalent to one)
* **Many** -- Allows many instances, no time-frame
* **Serial** -- Allows for many, time-separated, instances where it's sampled.  (ie, weight)
* **All** -- Allows for many, possibly time-separated, instances


### pkey Attibute

    pkey (key)
    
This adds an alternate (plural) key to the field definition (exact usage TBD).

*SINGLE* use only, others will be ignored

### Plural Attribute

    plural  (Plural form)
    
This is the plural form of the class, proper form.

*SINGLE* use only, others will be ignored.

### Short Notes Attribute

    notes   (Notes)
    
These are brief explanatory notes to help remember what this class is for.  Again, one line.

*SINGLE* use only, others will be ignored

### Tag Attribute

    tag (tag-key)

This denotes that this field is associated with the specified user-defined tag in the dataset.

*MULTIPLE* are permitted, all will be aggregated together, duplicates will be tolerated and ignored.



