CREATE TABLE Doodles (
        -- This ID will be the corresponding fragment ID
    id          INTEGER PRIMARY KEY,
    title       VARCHAR(255),
	icon        INTEGER
);

CREATE TABLE DoodleTag (
    doodle      INTEGER NOT NULL,
    tag         INTEGER NOT NULL,
    UNIQUE(doodle, tag)
);



