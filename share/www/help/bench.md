#!	Workbench

Bench, short for workbench, is the entire workspace that encompasses the user's work product.  

##	Cache




##	File

The workbench configuration is as `.bench` in a directory.  While it's expected to be in one of the work product directories, it's not required.

## Attributes

### Cache

The cache is a database used to track details from the files for faster lookups rather than parsing through thousands (or more)

	cache (path)  

Default is `.cache', this defines where the database is located.  As the cache is considered disposable, it does not need to reside on permanent storage.   See [Cache](cache.md) for more details.

