CREATE TABLE Nodes (
    id      INTEGER PRIMARY KEY,
    k       VARCHAR(255) NOT NULL UNIQUE,
    leaf    INTEGER
);

CREATE TABLE NClasses (
    id      INTEGER PRIMARY KEY,
    k       VARCHAR(255) NOT NULL UNIQUE,
    base    INTEGER
);

CREATE TABLE Edges (
    id      INTEGER PRIMARY KEY,
    src     INTEGER NOT NULL,
    tgt     INTEGER,
    type    VARCHAR(255)
);

CREATE TABLE EClasses (
    id      INTEGER PRIMARY KEY
    
);


