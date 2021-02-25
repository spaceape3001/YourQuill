#! Quill's "File" System

There are two sides tracking documents in the workspace.  (See [here](workspaces.md) for a description of workspaces.)  The physical side are the directories and files that reside on the drive.  The logical side are the folders and documents that are presented to the user.

## Logical Side

The logical side is what is presented to the user after data fusion has occurred.  In general, it's not case sensitive and will FUSE similar directories/files found.

### Workspace

The workspace is the root of the folders and documents, contains everything.  

### Folders

Folders are collections of documents and sub-folders.  Data fusion aggregates this all together.

### Documents

Documents are a "bit" of information about something, be it a page, an image, etc.

## Physical Side

The physical side is what resides in permanent disk storage.  Case sensitivity depends on the medium and operating system.  

### Root Directory

The root directories are the basis directories on the filesystem, they are specified in the workspace's quill file, either directly or indirectly.  (ie, other directories can import more).  

Order of the directories:

1. The primary quill's directory is ALWAYS first in the list
2. Other directories in the primary
3. Other directories imported from those other directories
4. Template directories

### Directories

Self-explanatory, these are the directories found in the permanent storage.  They're enumerated into the cache database.

### Fragments

Fragments are the files on the permanent storage.  Due to case and multiple root directories, there can be more than one "file" for a document, therefore, they're considered "fragments" to the document.


## Fusion

Fusion is bringing multiple fragments together to make one document.  In the instance of one fragment to a document, the fusion is straightforward, take the relevant file as the document.  However, for multiple fragments, the fusion is more tricky.

### Leafs

Leafs will be fused together to make a combined product.

### Images

Images cannot be fused, so the first file found will be used.

### Other Documents

Where practical, documents will be fused together, however, this will vary from format to format.  Binary files are less likely to be fused than text ones.






