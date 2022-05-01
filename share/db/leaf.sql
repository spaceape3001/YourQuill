
CREATE TABLE Leafs (
                -- This ID will be the corresponding document ID
	id			INTEGER PRIMARY KEY,
	k			VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
	atom        INTEGER DEFAULT 0,
	icon        INTEGER,
	title		VARCHAR(255) COLLATE NOCASE,
	abbr        VARCHAR(255) COLLATE NOCASE,
	brief       VARCHAR(255)
);

CREATE TABLE LTags (
    leaf        INTEGER NOT NULL,
    tag         INTEGER NOT NULL,
    UNIQUE(leaf, tag)
);


