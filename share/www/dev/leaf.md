#! Leaf

The leaf is the heart of the user-driven database.  
It represents a "node" of data in their creation.

##	File extension

Leaf file extension shall be ".leaf"

##	Keys

A key is simply the path to the defining file.  A key should consist of letters, numbers, and limited punctuation.

##	Files

Every key should be backed by one or more files.

##	Auxillary Data

Auxillary data should start with the key, either be immediately trailed by a file extension, or have an intermediate key (if required).

key.bio.png  would be an image "bio" for the specified person.

##  Attributes

### Single point definition

It's best to have all data have a single point of definition, even if it's "reversed".  While sometimes this will break down, the single point definition and auto-fill will keep crud and quirks to a minimum.


