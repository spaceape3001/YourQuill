#! Leaf Class

The ``*.cls`` is a file that describes a leaf class, one that adds fields to a leaf class.

## File

The leaf class file makes use of the [Standard File](stdfile.md).  It has the general syntax of
	
	%		Name
	%cls	class1 ... classN
	
	field1
	...
	fieldN
	

### Class Attributes

#### Class

	%cls	class1 ... classN

The ``as`` attribute declares the other class that this class is a subset of.  For instance, a class ``club`` would be a subset of ``group``.
	
#### Icon

    icon
    
Specifies the icon for the class.
	
#### Label

	label	Name
	plural  Name(s)

The ``label`` attribute declares what's shown to the user, while plural is the plural form.


### Fields

### Field Definitions

A field definition is either a data-class or an expression.  If empty, the default will be "string"


### Field Attributes

#### Also

	also	(field)
	
This specifies that this is also an alias for the other field.  Typically used when a primary attribute is desired.

#### Alias

	alias	(field)

This specifies the alias for this field.  (use sparingly)

#### Count

	count	(max)			[1]
	count	(min), (max)	[2]
	
This restricts the count to min to max, where the [1] assumes min=0.

#### Hidden

	hide		true

#### Infer

    infer       true
    
This causes the edge to be auto-infered.

#### Label
	
	label (text)
	
This specifies the label for the field.

#### Label-if

    label-if (text), (conditional)

This specifies the label for the field with conditionals

#### Substitute

    substitute  (field)
    
Specifies that the other field can be a substitute for this field, if attributeless.

#### Target

    target  (class), ... 
    
Specifies target types for this edge


