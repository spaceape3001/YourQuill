-- Values
CREATE TABLE Vals (
    id          INTEGER PRIMARY KEY,
    field       INTEGER,
    data        VARCHAR(255) COLLATE NOCASE,
    brief       VARCHAR(255),
    
    UNIQUE(field,data)    
);

CREATE TABLE ValTags (
    val         INTEGER,
    tag         INTEGER,
    
    UNIQUE(val,tag)
);

