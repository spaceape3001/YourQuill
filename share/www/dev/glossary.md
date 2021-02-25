#! Glossary

Because definitions can vary, we fix them.

*	**Attr**			In-code short-hand for attribute
*	**Attribute**		Binding key to a value/edge/sub-node
*	**Chain**			Linking of one workspace to another, incorporating its traits/data into itself.
*	**Def**				In-code short-hand for definition
*	**Definition**		User's expression of what's supposed to happen.
*   **Directory**       Directory, on filesystem, case sensitive
*	**Edge**			Pointing of one node to another
*	**Element**			Generic way of discribing "bit" of information, or meta data
*   **File**            File on the drive
*   **Folder**          Directory-like user-facing, case insensitive.
*	**Info**			In-code short-hand for information
*	**Information**		Meta-information, in-code
*	[**Key**](keys.md)	Terse text string identifying a particular element.  Keys are typically restricted to letters, numbers, and limited-punctuation.
*	**Leaf**			Leaf is the wiki-page level of node in the database, one file per leaf per role per workspace.  
*	**Node**			User data, describing whatever.  It can, optionally, have one or more leaf types associated with it
*	**Tag**				A terse/short edge pointing from one leaf to another, defined by the user.
*	**Template**		A predefined workspace, shipped with this software, that a user can leverage
*	**Value**			A string/value.  Structured value types must be defined in code!
*	**Wksp**			In-code short-hand for workspace.
*	[**Workspace**](workspaces.md)		User space, defined as a directory, where they can configure and/or create nodes.

##  In Code

*   **cc**              create in cache (ie, the key may or may not exist in the relevant table)
*   **create**          create in xml/file (ie, this is the persistent kind)
*   **exists**          checks the cache for existence
*   **get**             get in cache (if not, it fails.)


