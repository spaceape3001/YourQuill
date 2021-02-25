What is a type?  

Goal
    Full introspection so that I (& user) can script this.

General Categories:

    Data
    Enums
    Flags
    Units
    Vectors
    Objects
    Collections (ie, Sets, Maps, Vectors, etc)
    Pointer (ie shared/object)

Common Stuff
    Member data (ie. INDEPENDENT states that can be set individually)


I/O
    Saving member data should be sufficient for preserving the object/data
    
    
Types:
    Singular Data (needs I/O help)
        Simple Data
            String
            Integer
            Double
            Float
        Enums
        Units
    Composite Data
        Structures
            Points/Math
        Collections
            Flags (Set)
        Objects

Actions
    Storage
    Passing
    I/O
    
Pointers
    Object* should be auto-registered with variant

Fields/Properties
    Fields      Variant-based
    Properties  Object-based
    
    These are two words for the same thing, depending on whether it's a generic object or generic data (Object vs Variant).  Both can have state.
    
Obtrusive vs Unobtrusive
    This will be harmonized at the scripting engine.


I/O
    IO formatting/parsing is meant for SIMPLE types/constructs only, where it's unambiguous and common.  (ie, 1/2 for a fraction, for instance)
    


