#! Leafs

Leaf is the wiki page that we see, ends in a ``*.w`` extension.  It's expected these files will be hand-crafted for some time.  

##  File

The file is separated into two sections, the attribute section, and the body section.  Attributes are clumped at the top, the body comes after the first blank line.

##  Attributes

Attributes are left-aligned, with a key/value style of setup.

    (commands) [id] (key) (value)
    
Sub-attributes are indented.  While the number of whitespace is unimportant, it has to remain consistent (ie, python).

### Keys

Keys are expected to be alpha-numerical strings.  Start with a-z and may contain numbers/underscores.  The exception are system-defined attributes that are prefixed with '``%``'.  

### ID

Id is an optional text string, enclosed in the square brackets, following the same rules as keys, to allow for identification of attributes.  Note, these IDs should be unique to the node across all roots.  


### Modifiers

TBD, but modifiers are commands (punctuation-started) that will operate on attributes.  (ie, remove, and/or alter a previously declared attribute)

    +   [id] (key) (value)      Will add attributes (+ is redundant here)
    -   [id] (key) (value)      Will strike attributes (will not work on system attributes) 
    --  (key)                   will strike all previous attributes of this key (will not work on system attributes)
    
Periods in the keys will denote paths, so will operate on sub-attributes.


### Sub-attributes
   
Sub-attributes are declared by indentation


### Body

Format will be dictated by the ``%format`` (or ``%fmt``) attribute, however, the default interpretation will be markdown.


## System Attributes

System attributes are all prefixed with '``%``'.

### Title

	%	    (title)
	
This is the title to the wikipage.  Only the first one is honored.

### As/class

    %is     class1...classN
	%as	    class1...classN
	%class  class1...classN

This defines the classes this leaf is.

### Description

    %brief (description)
	%desc  (description)
	
Brief description for tables.

### Key

    %key    key
    
The key is what another leaf can use in [] to create a link, universally applied


### Notes

	%notes (notes)
	
General (short) notes, won't be listed

### Format

    %fmt    (format)
	%format	(format)
	
Format (default is markdown)

### Type (Sub-attribute)

    %type   (type)
    
This one is typically a sub-attribute, used to specify the type of an attribute.  This declaration will override whatever's listed in class(es) definitions.  Default type will be string (the most generic thing there is).  Type can be a standard data type or an atom class. 

(tbd, multiple declarations)



## Context

All context is assumed to be in markdown unless the format is otherwise declared



