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
    brief       VARCHAR(255),
    icon        INTEGER,
    is_owner    INTEGER DEFAULT 0,
    is_admin    INTEGER DEFAULT 0,
    is_writer   INTEGER DEFAULT 0,
    is_reader   INTEGER DEFAULT 0,
    is_guest    INTEGER DEFAULT 0
);


