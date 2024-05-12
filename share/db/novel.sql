
CREATE TABLE Novels (
        -- This ID will be the corresponding fragment ID
	id			INTEGER PRIMARY KEY,
	icon        INTEGER,
	title		VARCHAR(255) COLLATE NOCASE
);

CREATE TABLE NovelTag (
    novel       INTEGER NOT NULL,
    tag         INTEGER NOT NULL,
    UNIQUE(novel, tag)
);


