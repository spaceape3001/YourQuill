#! Keys

The "key" is a unique identifier of a property/attribute/etc.  It's to follow close to filename & XML & C++'s rules for identifiers.  Keys are to be case insensitive (even if the filesystem *is*).  

User-defined keys should not contain a ".", whereas auto-gen can.

## Sub-keys

Sub-keys are used when there's a qualification to a user-leaf.  (ie, various images, extra files, etc.)

For instance, 

`leaf.yq` is the leaf.  `leaf.png` is the thumbnail (shown everywhere), while `leaf.flag.png` would be it's "flag" subkey.

##	Directories

Directories are absolutely supported, and part of the key.  For instance, `dir/leaf` would map to the leaf in file `dir/leaf.yq`.



