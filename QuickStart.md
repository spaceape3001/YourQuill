# Quick Start

After building.

1.  Create a directory (documents is fine, or similar location) for your new project.
2.  Copy in an example yquill from ``share/examples``, rename it to `.yquill`` in your new directory (step #1)
3.  Edit ``.yquill`` parameters to suit your project
4.  Execute ``yquill <path-to-directory>``
5.  Navigate web-browser to ``http://localhost:25505`` (or whichever port number you decide to use)
    a.  Note, this *is* a web-server, so it'll be visible to everybody on your network.

## PORTS MUST BE UNIQUE 

If you've got multiple projects, they'll need multiple port numbers.  This is because only one application instance can bind to a particular port number.  Therefore, if you have multiple projects running concurrently, you'll need to have each project with its own dedicated port number.  There's plenty of them, so choose one between 1024-49151 that's not already in use by another application.  (If unsure to current port usage, a scanner like nmap can help.)

