--------------------------------------------------------------------------------
--
--  YOUR QUILL
--
--------------------------------------------------------------------------------


CREATE TABLE Atoms (
    id          INTEGER PRIMARY KEY,
    k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
        -- portion that's not part of the document
    sk          VARCHAR(255) COLLATE NOCASE,
    abbr        VARCHAR(255),
        -- left here until it's all switched over....
    doc         INTEGER NOT NULL,
    leaf        INTEGER NOT NULL DEFAULT 0,
        -- Parent atom (for composition)
    parent      INTEGER NOT NULL DEFAULT 0,
	icon        VARCHAR(255),
	brief       VARCHAR(255),
	is_edge     BOOL NOT NULL DEFAULT 0,
	    -- this is the title
	name        VARCHAR(255)
);


CREATE TABLE AtomClass (
        -- atom of interest
    atom    INTEGER NOT NULL,
        -- class of interest
    class   INTEGER NOT NULL,
    
        -- how direct the definition is (ie, how many other class definitions it must reach through.
    hops    INTEGER DEFAULT 0,
    
    UNIQUE(atom,class) ON CONFLICT IGNORE
);

CREATE TABLE AtomProperty (
    id      INTEGER PRIMARY KEY,
    atom    INTEGER NOT NULL,
    attr    INTEGER NOT NULL,
    
        -- child key (if needed)
    ck      VARCHAR(255),
    
        -- if empty, means the fields not been assigned
    field   INTEGER NOT NULL DEFAULT 0,
    
        -- class that applies
    class   INTEGER NOT NULL DEFAULT 0,
    
        -- child atom spawned
    child   INTEGER NOT NULL DEFAULT 0,
    
        -- if this is an edge, the originating source
    source  INTEGER NOT NULL DEFAULT 0,
    
        -- if this is an edge, the resulting target
    target  INTEGER NOT NULL DEFAULT 0,
    
    UNIQUE(atom,attr) ON CONFLICT IGNORE
);
CREATE TABLE AtomTag (
        -- atom of interest
    atom    INTEGER NOT NULL,
        -- tag of interest
    tag     INTEGER NOT NULL,
    UNIQUE(atom,tag) ON CONFLICT IGNORE
);

    --  note, the edge's parent is the source, always defined
CREATE TABLE AtomEdge (
    --  source atom
    source  INTEGER NOT NULL DEFAULT 0,
    
    --  edge atom
    edge    INTEGER NOT NULL,
    
    --  target atom
    target  INTEGER NOT NULL DEFAULT 0,
    
    --  attribute that triggered this edge
    attr    INTEGER NOT NULL DEFAULT 0,

    --  target specification (for when the target's not resolved)
    tgtspec VARCHAR(255)
);

