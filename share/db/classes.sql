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

    -- Dependencies as analyzed
CREATE TABLE ClassDepend (
    class       INTEGER NOT NULL,
    base        INTEGER NOT NULL,
    -- number of indirects (zero is what's in the file)
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class,base) ON CONFLICT IGNORE
);

    -- fields that are applied to this particular class (all of them)
CREATE TABLE ClassField (
    class       INTEGER NOT NULL,
    field       INTEGER NOT NULL,
    -- number of indirects (zero is what's in the file)
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class, field)
);

    -- Aliases as expanded upon (all of them)
CREATE TABLE ClassLookup (
    class       INTEGER NOT NULL,
    alias       VARCHAR(255) NOT NULL,
    
    -- zero is strongest, 
    priority    DECIMAL NOT NULL DEFAULT 0,
    UNIQUE(class, alias) ON CONFLICT IGNORE
);

    -- prefixes as defined by the class definiton
CREATE TABLE ClassPrefix (
    class       INTEGER NOT NULL,
    prefix      VARCHAR(255) NOT NULL COLLATE NOCASE,
    UNIQUE(class,prefix) ON CONFLICT IGNORE
);

    -- suffixes as defined by the class definiton
CREATE TABLE ClassSuffix (
    class       INTEGER NOT NULL,
    suffix      VARCHAR(255) NOT NULL COLLATE NOCASE,
    UNIQUE(class,suffix) ON CONFLICT IGNORE
);
    -- tags to this class
CREATE TABLE ClassTag (
    class       INTEGER NOT NULL,
    tag         INTEGER NOT NULL,
    UNIQUE(class,tag) ON CONFLICT IGNORE
);



    -- Aliases as defined in the class file
CREATE TABLE ClsDefAlias (
    class       INTEGER NOT NULL,
    alias       VARCHAR(255) NOT NULL,
    UNIQUE(class, alias) ON CONFLICT IGNORE
);

    -- prefixes as defined by the class definiton
CREATE TABLE ClsDefPrefix (
    class       INTEGER NOT NULL,
    prefix      VARCHAR(255) NOT NULL COLLATE NOCASE,
    UNIQUE(class,prefix) ON CONFLICT IGNORE
);

CREATE TABLE ClsDefReverse (
    class       INTEGER NOT NULL,
    reverse     INTEGER NOT NULL,
    UNIQUE(class,reverse) ON CONFLICT IGNORE
);

CREATE TABLE ClsDefSource (
    class       INTEGER NOT NULL,
    source      INTEGER NOT NULL,
    UNIQUE(class,source) ON CONFLICT IGNORE
);

    -- suffixes as defined by the class definiton
CREATE TABLE ClsDefSuffix (
    class       INTEGER NOT NULL,
    suffix      VARCHAR(255) NOT NULL COLLATE NOCASE,
    UNIQUE(class,suffix) ON CONFLICT IGNORE
);

CREATE TABLE ClsDefTarget (
    class       INTEGER NOT NULL,
    target      INTEGER NOT NULL,
    UNIQUE(class,target) ON CONFLICT IGNORE
);

    -- USES as defined in the class file
CREATE TABLE ClsDefUse (
    class       INTEGER NOT NULL,
    use         INTEGER NOT NULL,
    UNIQUE(class, use) ON CONFLICT IGNORE
);


--------------------------------------------------------------------------------
-- OLD
--------------------------------------------------------------------------------

CREATE TABLE CEdges (
    class   INTEGER NOT NULL,
    source  INTEGER NOT NULL,
    target  INTEGER NOT NULL,
    -- number of indirects (zero is what's in the file)
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class,source,target) ON CONFLICT IGNORE
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

CREATE TABLE CTargets (
    class       INTEGER NOT NULL,
    target      INTEGER NOT NULL,
    hops        INTEGER NOT NULL DEFAULT 0,
    UNIQUE(class,target) ON CONFLICT IGNORE
);


