CREATE TABLE Classes (
        -- ID is the document ID
    id          INTEGER PRIMARY KEY,
    k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
    edge        BOOLEAN NOT NULL DEFAULT 0,
	removed     BOOLEAN NOT NULL DEFAULT 0,
    name        VARCHAR(255),
    deps        INTEGER NOT NULL DEFAULT 0,
    icon        INTEGER NOT NULL DEFAULT 0,
    plural      VARCHAR(255),
    brief       VARCHAR(255)
);

CREATE TABLE CDefUse (
    id          INTEGER NOT NULL,
    base        INTEGER NOT NULL,
    UNIQUE(id,base) ON CONFLICT IGNORE
);

CREATE TABLE CDefRev (
    id          INTEGER NOT NULL,
    base        INTEGER NOT NULL,
    UNIQUE(id,base) ON CONFLICT IGNORE
);

CREATE TABLE CDefSrc (
    id          INTEGER NOT NULL,
    base        INTEGER NOT NULL,
    UNIQUE(id,base) ON CONFLICT IGNORE
);

CREATE TABLE CDefTgt (
    id          INTEGER NOT NULL,
    base        INTEGER NOT NULL,
    UNIQUE(id,base) ON CONFLICT IGNORE
);


CREATE TABLE CDepends (
    id          INTEGER NOT NULL,
    base        INTEGER NOT NULL,
	hops        INTEGER NOT NULL,
    UNIQUE(id,base) ON CONFLICT IGNORE
);

CREATE TABLE CReverses (
    id          INTEGER NOT NULL,
    rev         INTEGER NOT NULL,
	hops        INTEGER NOT NULL,
    UNIQUE(id,rev) ON CONFLICT IGNORE
);

CREATE TABLE CSources (
    id          INTEGER NOT NULL,
    src         INTEGER NOT NULL,
	hops        INTEGER NOT NULL,
    UNIQUE(id,src) ON CONFLICT IGNORE
);

CREATE TABLE CTargets (
    id          INTEGER NOT NULL,
    tgt         INTEGER NOT NULL,
	hops        INTEGER NOT NULL,
    UNIQUE(id,tgt) ON CONFLICT IGNORE
);


CREATE TABLE CFields (
    class       INTEGER NOT NULL,
    field       INTEGER NOT NULL,
	hops        INTEGER NOT NULL,
    UNIQUE(class, field) ON CONFLICT IGNORE
);
