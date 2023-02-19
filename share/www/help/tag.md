#! Tags

The tag is a way of binding leafs, atoms, and other similar user-generated information back to a single point of reference.  Tags can be specific plots, stories, etc -- it's up to the user to determine how these are to be exploited.

## Files

All tags are defined in the tags folder (<tt>/.tags</tt>), and they have a corresponding <tt>*.tag</tt> file extension.  This file follows the standard format, however, context is not currently supported.

### Tag Name Attribute

    name    (Name of the tag)
    
This attribute defines the proper name for the tag, this can correct for capitalization, and/or characters not available on the filesystem.  Alternatively, it can be a full name, whereas it's easier to write a shorter name in all the files.

*SINGLE* use only, others will be ignored


### Brief Description Attribute

    brief   (Description)
    
This is a brief one-liner description for what this tag is about (story, capability, etc).

*SINGLE* use only, others will be ignored

### Short Notes Attribute

    notes   (Notes)
    
These are brief explanatory notes to help remember what this tag is for.  Again, one line.

*SINGLE* use only, others will be ignored

### Reference Leaf Attribute

    leaf    (key)
    
This attribute allows for the tag to be tied back to a specific leaf wiki page.

*SINGLE* use only, others will be ignored


