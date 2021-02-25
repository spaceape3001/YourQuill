#! Workspaces and Projects

Overarching goal is to allow the user to express themselves, be it a novel, play, movie, etc.
We simply allow them to do it.

## 	Project

The project is a concept, not expressed in the codebase, but here.  
It's simply the "project" that the user is working on, whether its just one big 
item, or a series of items set in the same "universe".

##	Workspace

The workspace is our concept, it realizes the project, it encompasses all the 
work being done on the project, as defined by the user.  
There is *no* requirement that a project be restricted to a single workspace, but 
instead, it can be spread across multiple workspaces -- this is what chaining is for.

##  Chaining

This causes one workspace to incorporate another, allowing for the workspace to utilize the assets of the other.  Reasons for chaining can include:

*	**Intensity** of some characteristic requires segregation of material (ie MPAA ratings), so you have a "higher" leveraging a "lower" intensity workspace.
*	**Fanwork** where you base it off another work, so your project leverages a canon-based workspace
*	**Releasibility** where you have a public-released workspace, and a private/under-development workspace.
*	**Crossovers** where you produce one work leveraging two or more other projects w/o actually binding those other projects.

Whether another workspace can be written to will be determined by the user in the policy settings.

By default, chaining will merge the data together.  Exclusions must be applied to keep this from happening.

###	Templates

Templates are a form of chaining, where the top-level workspace information (name/author/etc) is not merged, but the definitions/data are.  This allows for generalized areas of fiction, say "magic" or "medical" to be accessible to projects.

###	Access/DataRoles

This is the (current) mechanism to control levels of access, governed by the root workspace.


1.  *Read Only*, where the other workspace can only be read from
2.  *Read/Write*, where the other workspace can be read and/or written to.
3.  *Write First*, where this other workspace is the *DEFAULT* for writing.  Note, only *ONE* other workspace can be marked as *Write First*.

##	Documents are the Database

Each workspace *is* an unstructured database of documents, including XML, images, novels, etc.  
These documents *are* the authoritative source of data for each workspace.  

##	Duplicates across chains

How each document type merges in the case of duplicates is handled on a 
type-by-type basis, and this can be configured by the user.

##	Template workspace

Template workspaces are located under the share directory "template", and 
they'll be auto-chained in at every run so they can capture any improvements
that may be had.

##	Cache

Every workspace will have an independent cache database.  
It will cache its material and chain material.   
This separation will avoid software-based "spillage" between databases.

### Cache is disposable

A cache can be deleted by the user between runs, so *no data loss should occur* in this situation.  
Instead, the cache will simply be rebuilt (ideally a fast process, but can move at the speed it moves along).

### Cache is relocatable

Default location is $wksp/.cacheDB

The user can decide to relocate the cache to a different location, which will take effect at the next startup.  This will be useful int he case of read-only medium, or putting the cache into a better location (ie, RAM drive, fast SSD, etc).

*TBD, do we make this an app-wide setting?*

##	Logging

Logging is on by default, will go both to a console and/or file.  (A user cannot fully squelch the logging, but can restrict it to emergency-level logging only.)  Logging directory is user-configurable, by default is $wksp/.logs

##	Spillage

Spillage is the leakage between workspaces, something showing up where it shouldn't (ie, a private thing in public workspace, or a higher-intensity in a lower-intensity thing).  While software based spillage is mitigated by keeping the cache databases separate, user-caused spillage will be something to be on the look out for.

**User-level spillage will need to be mitigated**

While we can't completely prevent user-level spillage, we can give warnings/prompts; make it clear *where* a document is going.

##	Data

It'd be nice to move the workspace characteristics toward a generic attributable mechanism.



