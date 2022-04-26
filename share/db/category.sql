CREATE TABLE Categories (
    -- category ID (it'll be defining document ID)
    id      INTEGER PRIMARY KEY,
    -- category key
    k       VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
    
    -- category icon
    icon    INTEGER NOT NULL DEFAULT 0,
    
    name    VARCHAR(255),
    
    brief   VARCHAR(255)
);

