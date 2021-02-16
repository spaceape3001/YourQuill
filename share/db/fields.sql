

CREATE TABLE Fields (
        -- ID is the document ID
    id          INTEGER PRIMARY KEY,
    k           VARCHAR(255) NOT NULL COLLATE NOCASE UNIQUE,
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
	removed     BOOLEAN NOT NULL DEFAULT 0
);


CREATE TABLE FAlias (
    field       INTEGER NOT NULL,
    alias       VARCHAR(255) NOT NULL COLLATE NOCASE UNIQUE
);

CREATE TABLE FTypes (
    field       INTEGER NOT NULL,
    type        VARCHAR(255) NOT NULL COLLATE NOCASE,
    UNIQUE(field,type) ON CONFLICT IGNORE
);
