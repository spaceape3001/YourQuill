--------------------------------------------------------------------------------
--
--  YOUR QUILL
--
--------------------------------------------------------------------------------

    --  =============================================================
    --      USERS
    --  =============================================================

CREATE TABLE Users (
    id          INTEGER PRIMARY KEY,
    k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
    name        VARCHAR(255) COLLATE NOCASE,
    icon        INTEGER
);

CREATE TABLE UserGroups (
    id          INTEGER PRIMARY KEY,
    k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
    name        VARCHAR(255) COLLATE NOCASE,
    icon        INTEGER
);


CREATE TABLE UGLink (
    uid         INTEGER NOT NULL,
    gid         INTEGER NOT NULL,
    UNIQUE(uid, gid) ON CONFLICT IGNORE
);

