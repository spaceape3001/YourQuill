#! Projects

Projects are, as the tin suggests, the "projects" of this database.  A project can have one or more stories.

## Config File

Projects make use of the [Standard File](stdfile.md).

### Attributes

#### Abbreviation

	abbr (abbr)

This is the "short" version of the project's name.

#### Author

	author (author)
	
This is the author of the project.

#### Brief

	brief (brief)

This defines a "brief" description.  Keep it short.


#### Name

	name (name)

This specifies what the name of the project is.

#### Templates

	templates name1, name2, .... nameN

This specifies the keys for templates.  Note, standard templates begin with ".".

### Content

Content for the project is simple, these are concatenated together to form the full contents on the project description page.

## Template Projects

Template projects are ones that either reside in a bench designated directory *OR* the ***YourQuill** share template directory.

### Keys

Template keys have "." prepended to denote it as a template.

### Naming Restrictions

Templates are restricted from having reserved keywords (as in, bad things will happen if violated).  Currently:

+	`.api`
+	`.cache`
+	`.copyright`
+	`.css`
+	`.dev`
+	`.footer`
+	`.help`
+	`.js`
+	`.logs`
+	`.users`
+	`.server.ini`



