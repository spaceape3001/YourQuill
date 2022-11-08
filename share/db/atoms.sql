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
    leaf        INTEGER DEFAULT 0,
        -- Parent atom (for composition)
    parent      INTEGER DEFAULT 0,
	icon        VARCHAR(255),
	brief       VARCHAR(255),
	is_edge     BOOL DEFAULT 0,
	    -- this is the title
	name        VARCHAR(255)
);

CREATE TABLE AAttributes (
    atom    INTEGER NOT NULL,
    attr    INTEGER NOT NULL,
    
    UNIQUE(atom,attr) ON CONFLICT IGNORE
);

CREATE TABLE AClasses (
        -- atom of interest
    atom    INTEGER NOT NULL,
        -- class of interest
    class   INTEGER NOT NULL,
    
        -- how direct the definition is (ie, how many other class definitions it must reach through.
    hops    INTEGER DEFAULT 0,
    
    UNIQUE(atom,class) ON CONFLICT IGNORE
);

CREATE TABLE ATags (
        -- atom of interest
    atom    INTEGER NOT NULL,
        -- tag of interest
    tag     INTEGER NOT NULL,
    UNIQUE(atom,tag) ON CONFLICT IGNORE
);

    --  note, the edge's parent is the source, always defined
CREATE TABLE AEdges (
    --  source class
    source  INTEGER NOT NULL,
    
    --  edge class
    edge    INTEGER NOT NULL,
    
    --  target class
    target  INTEGER NOT NULL DEFAULT 0,

    --  target specification (for when the target's not resolved)
    tgtspec VARCHAR(255)
);

