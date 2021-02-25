#! Standard Files

***YourQuill*** makes use of a standard file format.  It's layout is expected to follow a common layout.

	; 	Header comment  (optional)
	key value 			(required-attributes) 

	;	Middle comment  (optional)
	Content

	;	Footer comment  (optional)

## Uses

The following use standard file format:

+	[Bench](bench.md) &mdash; The "bench" file that controls the server.
+	[Project](project.md) &mdash; The "project" file.

## Comments

Comments in these files start with ````;```` at the **start** of the line.  These are segregated into three main blocks, the header, the middle, and the footer.  Upon load/save, all comments will be pushed into these expected locations.


## Attributes

+	Key/Value pairs
+	Commas can be used to separate multiple values
+	Multiple values depend on file/attribute specific.
+	Double quotes can enclose a value to preserve control characters (ie, commas, double spaces, etc).
+   ***NO BLANK LINES*** &mdash; the parser uses the blank line to terminate attribute processing and move onto the content line
+	At least ***ONE*** key/value pair is required, otherwise, the content will not be read!

## Blank Line

At least **ONE** blank line must exist between the attributes and contents.

## Contents

This is anything else after that one blank line that isn't considered a comment.

