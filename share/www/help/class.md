#! Class

The class is short for "classification" which means type of [Atom](atom.md).  A class can  contain one or more fields, which are expected attributes for atoms.

## Files

All classes are defined in the classes folder (`/.classes`), and they have a corresponding `*.class` file extension.  This file follows the standard format, however, context is not currently supported.

### Key

The filename of the class definition is taken as the class key.  This is case insensitive, and should be a valid [key](key.md).

### Class Name Attribute

    name    (Name of the class)

This attribute defines the proper name for the class, this can correct for capitalization, and/or characters not available on the filesystem.  Alternatively, it can be a full name, whereas it's easier to write a shorter name in all the files.

*SINGLE* use only, others will be ignored

### Alias Attribute

    alias   (alias)
    
Alternative key for the class, can be used anywhere.

*MULTIPLE* are permitted, all will be aggregated together, duplicates will be tolerated and ignored.

### Brief Description Attribute

    brief   (Description)
    
This is a brief one-liner description for what this class is about.

*SINGLE* use only, others will be ignored

### Folder Attribute

    folder  (Folder key)
    
The folder is (.... TBD ....)

*SINGLE* use only, others will be ignored

### Is attribute

    is      (class)
    
The <tt>is</tt> attribute denotes other classes this class leverages on.  For example, in the plot template, a Character *is* an entity, and you'd see.  

    name    Character
    is      Entity
    

*MULTIPLE* are permitted, all will be aggregated together, duplicates will be tolerated and ignored.


### Plural Attribute

    plural  (Plural form)
    
This is the plural form of the class, proper form.

*SINGLE* use only, others will be ignored.


### Reverse Attribute

    reverse (class-key)

This is an edge class that can be considered the "reverse" instance of this specification.  For instance, a child would have a reverse of parent.  

*MULTIPLE* are permitted, all will be aggregated together, duplicates will be tolerated and ignored.

### Short Notes Attribute

    notes   (Notes)
    
These are brief explanatory notes to help remember what this class is for.  Again, one line.

*SINGLE* use only, others will be ignored


### Source Attribute

    source (class-key)
    
Denotes that this class is an edge, and a valid source atom has the specified class associated with it.

*MULTIPLE* are permitted, all will be aggregated together, duplicates will be tolerated and ignored.
    
### Target Attribute

    target (class-key)
    
Denotes that this class is an edge, and a valid target atom has the specified class associated with it.

*MULTIPLE* are permitted, all will be aggregated together, duplicates will be tolerated and ignored.

    

