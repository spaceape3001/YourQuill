Files

On a PC for a general author's database, files are excellent, however, they can be wasteful in terms of inodes, disc-space, etc.  Therefore, an two types of collective file archives are needed:

1) read-only "packages" for templates
2) read-write for general usage (can be DB backed?)

Thus, the "search path" needs to accommodate this (and allow for variable length filenames, or we impose a restriction?)

