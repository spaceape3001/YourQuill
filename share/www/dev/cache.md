#! Cache

The server employs a SqlLite database for tracking the aspects of the entire workproduct.  This database is used by most of server for fast lookups rather than parsing through thousands (or more) files.  

***Cache is Disposable!!!***  The cache is auto-generated from the user's files and is therefore considered disposable, it may be safely deleted wheneve the server isn't running.  This implies that the SqlLite database should *NOT* be sole source.



