
    -- This stores the attributes in a file.
    -- We need to know what's changed, therefore retain this info
CREATE TABLE Attributes (
        -- ID number
    id          INTEGER PRIMARY KEY,
        -- defining document
    doc         INTEGER DEFAULT 0 NOT NULL,
        -- parent attribute (it's a tree)
    parent      INTEGER DEFAULT 0 NOT NULL,
        -- index in the parent/file
    idx         INTEGER,
        -- key
    k           VARCHAR(255) NOT NULL COLLATE NOCASE,
        -- user defined ID (file)
    uid         VARCHAR(255) COLLATE NOCASE,
        -- value of the attribute (generic text)
    value       VARCHAR(255) COLLATE NOCASE
);

