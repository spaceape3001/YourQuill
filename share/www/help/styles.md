#! Styles and Themes

Styles and themes are controlled by Cascading Style Sheets (CSS).  See [W3Schools](https://www.w3schools.com/css/) for details.

*Your Quill* has several mechanisms to combine files to produce a cohesive stylesheet.  It does so by combining:

1. **ALL** ``.css`` found in the base of the root directories, in order.
1. ``std/css`` in the share directory.

On step one, it scans the CSS for colors for consistent use.  Anything in the body is applied as a "variable" for the contents of ``std/css``.  

For instance, all you need to do to make white text on black is:

````
body {
   color: white;
   background-color: black;
}
````


