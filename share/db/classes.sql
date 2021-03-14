CREATE TABLE Classes (
        -- ID is the document ID
    id          INTEGER PRIMARY KEY,
    k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
    edge        BOOLEAN NOT NULL DEFAULT 0,
	removed     BOOLEAN NOT NULL DEFAULT 0,
    name        VARCHAR(255),
    icon        INTEGER NOT NULL DEFAULT 0,
    
        -- dependency graph!
    deps        INTEGER,
    plural      VARCHAR(255),
    brief       VARCHAR(255)
);

CREATE TABLE CDepends (
    class       INTEGER NOT NULL,
    base        INTEGER NOT NULL,
    UNIQUE(class,base) ON CONFLICT IGNORE
);

CREATE TABLE CDependsDef (
    class       INTEGER NOT NULL,
    base        INTEGER NOT NULL,
    UNIQUE(class,base) ON CONFLICT IGNORE
);


CREATE TABLE CFields (
    class       INTEGER NOT NULL,
    field       INTEGER NOT NULL,
    UNIQUE(class, field) ON CONFLICT IGNORE
);

CREATE TABLE CFieldsDef (
    class       INTEGER NOT NULL,
    field       INTEGER NOT NULL,
    UNIQUE(class, field) ON CONFLICT IGNORE
);

CREATE TABLE CReverses (
    class       INTEGER NOT NULL,
    reverse     INTEGER NOT NULL,
    UNIQUE(class,reverse) ON CONFLICT IGNORE
);

CREATE TABLE CReversesDef (
    class       INTEGER NOT NULL,
    reverse     INTEGER NOT NULL,
    UNIQUE(class,reverse) ON CONFLICT IGNORE
);

CREATE TABLE CSources (
    class       INTEGER NOT NULL,
    source      INTEGER NOT NULL,
    UNIQUE(class,source) ON CONFLICT IGNORE
);

CREATE TABLE CSourcesDef (
    class       INTEGER NOT NULL,
    source      INTEGER NOT NULL,
    UNIQUE(class,source) ON CONFLICT IGNORE
);

CREATE TABLE CTargets (
    class       INTEGER NOT NULL,
    target      INTEGER NOT NULL,
    UNIQUE(class,target) ON CONFLICT IGNORE
);

CREATE TABLE CTargetsDef (
    class       INTEGER NOT NULL,
    target      INTEGER NOT NULL,
    UNIQUE(class,target) ON CONFLICT IGNORE
);

CREATE TABLE CTags (
    class       INTEGER NOT NULL,
    tag         INTEGER NOT NULL,
    UNIQUE(class,tag) ON CONFLICT IGNORE
);

