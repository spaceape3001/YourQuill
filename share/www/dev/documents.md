#! Documents (in Cache)

A document is a keyed resource, and it's formed by combining one or more fragments into the same item.  Depending on the resource type, this fusion will either be a true combination, *OR* taking the first valid fragment.  (ie, a leaf will be merged, a document image will only use the first fragment.)

A document can be comprised of (tbd) components:
        
1. `[folder]/[skeyb]`
2. `[folder]/[skeyb].[extension]`
3. `[folder]/[skeyb].[middle].[extension]`
        
A document always resides in a folder, even if it's the top folder.

A similar document is one that shares the same folder & skeyb, but can vary in middle and extension.

