CREATE TABLE Classes (
        -- ID is the document ID
    id          INTEGER PRIMARY KEY,
    k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
    edge        BOOLEAN NOT NULL DEFAULT 0,
	removed     BOOLEAN NOT NULL DEFAULT 0,
    name        VARCHAR(255),
    icon        INTEGER NOT NULL DEFAULT 0,
    category    INTEGER NOT NULL DEFAULT 0,
    binding     VARCHAR(255),
    
        -- dependency graph!
    deps        INTEGER,
    plural      VARCHAR(255),
    brief       VARCHAR(255)
);

CREATE TABLE CAlias (
    class       INTEGER NOT NULL,
    alias       VARCHAR(255) NOT NULL,
    UNIQUE(class, alias)
);

CREATE TABLE CDepends (
    class       INTEGER NOT NULL,
    base        INTEGER NOT NULL,
    -- number of indirects (zero is what's in the file)
    hops        INTEGER NOT NULL,
    UNIQUE(class,base) ON CONFLICT IGNORE
);

CREATE TABLE CEdges (
    class   INTEGER NOT NULL,
    source  INTEGER NOT NULL,
    target INTEGER NOT NULL,
    -- number of indirects (zero is what's in the file)
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class,source,target) ON CONFLICT IGNORE
);

CREATE TABLE CFields (
    class       INTEGER NOT NULL,
    field       INTEGER NOT NULL,
    -- number of indirects (zero is what's in the file)
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class, field)
);

-- alternative keys
CREATE TABLE CLookup (
    class       INTEGER NOT NULL,
    k           VARCHAR(255) NOT NULL COLLATE NOCASE,
    priority    INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class,k)
);

CREATE TABLE CPrefix (
    class       INTEGER NOT NULL,
    prefix      VARCHAR(255) NOT NULL,
    UNIQUE(class, prefix)
);

CREATE TABLE CReverses (
    class       INTEGER NOT NULL,
    reverse     INTEGER NOT NULL,
    -- number of indirects (zero is what's in the file)
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class,reverse) ON CONFLICT IGNORE
);

CREATE TABLE CSources (
    class       INTEGER NOT NULL,
    source      INTEGER NOT NULL,
    -- number of indirects (zero is what's in the file)
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class,source) ON CONFLICT IGNORE
);

CREATE TABLE CSuffix (
    class       INTEGER NOT NULL,
    suffix      VARCHAR(255) NOT NULL,
    UNIQUE(class, suffix)
);

CREATE TABLE CTags (
    class       INTEGER NOT NULL,
    tag         INTEGER NOT NULL,
    UNIQUE(class,tag) ON CONFLICT IGNORE
);

CREATE TABLE CTargets (
    class       INTEGER NOT NULL,
    target      INTEGER NOT NULL,
    -- number of indirects (zero is what's in the file)
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class,target) ON CONFLICT IGNORE
);


