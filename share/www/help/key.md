#! Keys

A "key" is a unique identifier of a property, class, tag, etc.  It's intended to follow close to filename, XML, & C++'s rules for identifiers.  Keys are case INSENSITIVE, even if the filesystem *is* case sensitive.  As a general rule, keys should consist of letters, digits, and an underscore; however, slashes are used for paths, and the period '.' is used for sub-keys.

## Sub-keys

Sub-keys are used when there's a qualification to something.  (ie, various images, extra files, etc.)

For instance, 

`leaf.yq` is the leaf. `leaf.png` is the thumbnail (shown everywhere), while `leaf.flag.png` would be it's "flag" subkey.

## Directories

Depending on the specific resource, directories may or may not be supported as part of the keys.  When they are, they are combined with slashes.  For instance, `dir` and `leaf` would combine to be `dir/leaf` and its document key would be `dir/leaf.y`.

