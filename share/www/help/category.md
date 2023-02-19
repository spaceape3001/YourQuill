#! Categories

A category is a way to organize fields in an atom class.  Currently the restriction is one per field.

## Files

All categories are defined in the categories folder (<tt>/.categories</tt>), and they have a corresponding <tt>*.cat</tt> file extension.  This file follows the standard format, however, context is not currently supported.

### Key

The filename of the category definition is taken as the category key.  This is case insensitive, and should be a valid [key](key.md).

### Category Name Attribute

    name    (Name of the tag)

This attribute defines the proper name for the category, this can correct for capitalization, and/or characters not available on the filesystem.  Alternatively, it can be a full name, whereas it's easier to write a shorter name in all the files.

*SINGLE* use only, others will be ignored

### Brief Description Attribute

    brief   (Description)
    
This is a brief one-liner description for what this category is about.

*SINGLE* use only, others will be ignored

### Short Notes Attribute

    notes   (Notes)
    
These are brief explanatory notes to help remember what this category is for.  Again, one line.

*SINGLE* use only, others will be ignored


