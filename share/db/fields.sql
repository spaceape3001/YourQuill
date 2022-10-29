
--  Fields as presented/processed
CREATE TABLE Fields (
    -- ID is the document ID
    id          INTEGER PRIMARY KEY,

    -- this is the full field key, whether it's name or character.name
    k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
    
    -- class of principal definition, if it's singular, zero otherwise
    class       INTEGER NOT NULL DEFAULT 0,

    -- this is the key that's used within the class (ie "name" for "character.name")
    ck          VARCHAR(255) COLLATE NOCASE,
    
    -- nominal type
    expected    VARCHAR(255) COLLATE NOCASE,

    -- icon
    icon        INTEGER NOT NULL DEFAULT 0,

    -- name of the field
    name        VARCHAR(255) COLLATE NOCASE,
    
    -- plural of the field
    plural      VARCHAR(255) COLLATE NOCASE,
    
    -- plural (key) of the field
    pkey        VARCHAR(255) COLLATE NOCASE,
    
    -- brief of the field
    brief       VARCHAR(255),
    
    -- multi capability of the field
    multi       INTEGER NOT NULL DEFAULT 0,
    
    -- restriction of the field
    restrict    INTEGER NOT NULL DEFAULT 0,
    
    -- Category TBD
    category    INTEGER NOT NULL DEFAULT 0,
    
        -- SQL Table for atom/value associations (TBD)
    dbt         VARCHAR(255),

        -- SQL Table for values (TBD)
    dbv         VARCHAR(255),
    
        -- max count per atom (zero is unlimited)
    maxcnt      INTEGER NOT NULL DEFAULT 0,
    
        -- true if this field's been removed
	removed     BOOLEAN NOT NULL DEFAULT 0,
	
	    -- true if this field's been marked any-class (thus on all)
	anycls      BOOLEAN NOT NULL DEFAULT 0
);

-- INSERT INTO Fields (id, class, k, name, anycls) VALUES (1, 0, '%', 'Title', 1);

-- Defined classes this field applies to
--CREATE TABLE FDefClass (
--    field       INTEGER NOT NULL,
--    class       INTEGER NOT NULL,
--    UNIQUE(field,class)
--);

CREATE TABLE FAlias (
    field       INTEGER NOT NULL,
    alias       VARCHAR(255) NOT NULL COLLATE NOCASE,
    UNIQUE(field, alias)
);

-- note string is always implicit
CREATE TABLE FDataTypes (
    field       INTEGER NOT NULL,
        -- this comes out of the meta system
    type        INTEGER NOT NULL,
    UNIQUE(field,type)
);

CREATE TABLE FAtomTypes (
    field       INTEGER NOT NULL,
    class       INTEGER NOT NULL,
    hops        INTEGER NOT NULL,
    UNIQUE(field,class)
);

CREATE TABLE FTags (
    field       INTEGER NOT NULL,
    tag         INTEGER NOT NULL,
    UNIQUE(field, tag) 
);

