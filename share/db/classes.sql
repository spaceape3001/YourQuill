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
    url         VARCHAR(255),
    devurl      VARCHAR(255),
    
        -- dependency graph ID!
    deps        INTEGER,
    plural      VARCHAR(255),
    brief       VARCHAR(255)
);

    -- USES as defined in the class file
CREATE TABLE ClsUse (
    class       INTEGER NOT NULL,
    use         INTEGER NOT NULL,
    UNIQUE(class, use) ON CONFLICT IGNORE
);

    -- Aliases as defined in the class file
CREATE TABLE ClsAlias (
    class       INTEGER NOT NULL,
    alias       VARCHAR(255) NOT NULL,
    UNIQUE(class, alias) ON CONFLICT IGNORE
);

    -- Dependencies as analyzed
CREATE TABLE ClsDepend (
    class       INTEGER NOT NULL,
    base        INTEGER NOT NULL,
    -- number of indirects (zero is what's in the file)
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class,base) ON CONFLICT IGNORE
);

CREATE TABLE CEdges (
    class   INTEGER NOT NULL,
    source  INTEGER NOT NULL,
    target  INTEGER NOT NULL,
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

CREATE TABLE ClsTag (
    class       INTEGER NOT NULL,
    tag         INTEGER NOT NULL,
    UNIQUE(class,tag) ON CONFLICT IGNORE
);

CREATE TABLE CTargets (
    class       INTEGER NOT NULL,
    target      INTEGER NOT NULL,
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class,target) ON CONFLICT IGNORE
);


