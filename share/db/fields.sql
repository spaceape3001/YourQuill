
--  Fields as presented/processed
CREATE TABLE Fields (
        -- ID is the document ID
    id          INTEGER PRIMARY KEY,
    class       INTEGER NOT NULL,
    k           VARCHAR(255) NOT NULL,
    type        VARCHAR(255),
        -- Atom class, not an atom itself
    icon        INTEGER NOT NULL DEFAULT 0,
    pkey        VARCHAR(255),
    name        VARCHAR(255),
    plural      VARCHAR(255),
    brief       VARCHAR(255),
    multi       VARCHAR(255),
    restrict    VARCHAR(255),
    category    VARCHAR(255),
        -- SQL Table for atom/value associations
    dbt         VARCHAR(255),
        -- SQL Table for values
    dbv         VARCHAR(255),
    max         INTEGER NOT NULL DEFAULT 0,
	removed     BOOLEAN NOT NULL DEFAULT 0,
	
	UNIQUE(class,k) ON CONFLICT IGNORE
);

INSERT INTO Fields (class, k, name) VALUES (0, '%', 'Title');


CREATE TABLE FAlias (
    field       INTEGER NOT NULL,
    alias       VARCHAR(255) NOT NULL COLLATE NOCASE,
    UNIQUE(field, alias)
);

CREATE TABLE FDataTypes (
    field       INTEGER NOT NULL,
    type        VARCHAR(255) NOT NULL COLLATE NOCASE,
    UNIQUE(field,type) ON CONFLICT IGNORE
);

CREATE TABLE FAtomTypes (
    field       INTEGER NOT NULL,
    class       INTEGER NOT NULL,
    UNIQUE(field,class) ON CONFLICT IGNORE
);

CREATE TABLE FTags (
    field       INTEGER NOT NULL,
    tag         INTEGER NOT NULL,
    UNIQUE(field, tag) ON CONFLICT IGNORE
);

