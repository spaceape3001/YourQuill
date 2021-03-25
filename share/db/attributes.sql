
    -- This stores the attributes in a file
CREATE TABLE Attributes (
        -- ID number
    id          INTEGER PRIMARY KEY,
        -- defining document
    doc         INTEGER DEFAULT 0,
        -- parent attribute (it's a tree)
    parent      INTEGER DEFAULT 0,
        -- index in the parent/file
    idx         INTEGER,
        -- user defined ID (file)
    uid         VARCHAR(255)          COLLATE NOCASE,
        -- key
    k           VARCHAR(255) NOT NULL COLLATE NOCASE,
    value       VARCHAR(255)          COLLATE NOCASE
);

