
CREATE TABLE Atoms (
    id          INTEGER PRIMARY KEY,
    k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
    abbr        VARCHAR(255),
        -- left here until it's all switched over....
    doc         INTEGER NOT NULL,
    leaf        INTEGER DEFAULT 0,
        -- Parent atom (for composition)
    parent      INTEGER DEFAULT 0,
	icon        VARCHAR(255),
	brief       VARCHAR(255),
	name        VARCHAR(255)
);

    --  All documents that instantiate this atom (can be plural)
CREATE TABLE ADocuments (
    atom        INTEGER NOT NULL,
    doc         INTEGER NOT NULL,
    UNIQUE(atom,doc) ON CONFLICT IGNORE
);

CREATE TABLE AClasses (
        -- atom of interest
    atom    INTEGER NOT NULL,
        -- document where this was defined
    doc     INTEGER NOT NULL,
        -- class of interest
    class   INTEGER NOT NULL,
    UNIQUE(atom,doc,class) ON CONFLICT IGNORE
);

CREATE TABLE ATags (
        -- atom of interest
    atom    INTEGER NOT NULL,
        -- document this was defined
    doc     INTEGER NOT NULL,
        -- tag of interest
    tag     INTEGER NOT NULL,
    UNIQUE(atom,doc,tag) ON CONFLICT IGNORE
);

    --  note, the edge's parent is the source, always defined
CREATE TABLE AEdges (
    atom    INTEGER NOT NULL UNIQUE,
    target  VARCHAR(255),
    tgtid   INTEGER
);

