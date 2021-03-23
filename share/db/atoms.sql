
CREATE TABLE Atoms (
    id          INTEGER PRIMARY KEY,
    k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
    abbr        VARCHAR(255),
    leaf        INTEGER DEFAULT 0,
        -- Parent atom (for composition)
    parent      INTEGER DEFAULT 0,
	icon        VARCHAR(255),
	brief       VARCHAR(255),
	title       VARCHAR(255)
);

    --  All documents that instantiate this atom (can be plural)
CREATE TABLE ADocuments (
    atom        INTEGER NOT NULL,
    doc         INTEGER NOT NULL,
    UNIQUE(atom,doc) ON CONFLICT IGNORE
);

CREATE TABLE AClasses (
    atom    INTEGER NOT NULL,
    class   INTEGER NOT NULL,
    UNIQUE(atom,class) ON CONFLICT IGNORE
);

CREATE TABLE ATags (
    atom    INTEGER NOT NULL,
    tag     INTEGER NOT NULL,
    UNIQUE(atom,tag) ON CONFLICT IGNORE
);

CREATE TABLE AEdges (
    atom    INTEGER NOT NULL UNIQUE,
    source  INTEGER,
    target  INTEGER
);

