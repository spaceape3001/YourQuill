#! Users

The user is the human (or other sentient lifeform) accessing the web-site.

## Files

Users have a corresponding file in the users folder (`/.users`) with the file extension `*.user`.  This file follows the standard format.  It's bio constitutes the context for the file.

### Key

The filename of the user definition is taken as the user key.  This is case insensitive, and should be a valid [key](key.md).


### User Name Attribute

    name    (Name of the User)

This attribute defines the proper name for the user, this can correct for capitalization, and/or characters not available on the filesystem.  Alternatively, it can be a full name, whereas it's easier to write a shorter name in all the files.

*SINGLE* use only, others will be ignored


### Brief Description Attribute

    brief   (Description)
    
This is a brief one-liner description for this user.

*SINGLE* use only, others will be ignored

### Permissions Attribute

    permission  values
    
This is a comma separated list of what the user's permissions.

* **Owner** -- the owner of the installation
* **Admin** -- Administrator of the installation
* **Writer** -- An author
* **Reader** -- A reader

### Authentication

    authentication  (type)

This designates how the user will be vetted.  

#### AutoAccept

    authentication AutoAccept
    
This designates the user as being able to be logged in with ANY password.  Outside of a guest account or testing, this is inadvisable.

#### AutoReject

    authentication AutoReject
    
This rejects ALL attempts to log in with the user's name, regardless of password.

#### Simple

    authentication Simple
        password (some password)
    
This requires a simple plain text password.  Currently for testing purposes, this will eventually be replaced with stronger measures.






