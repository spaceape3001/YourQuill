CREATE TABLE Styles (
    id          INTEGER PRIMARY KEY,
    k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
    name        VARCHAR(255),
    brief       VARCHAR(255),
    icon        INTEGER NOT NULL DEFAULT 0
);

