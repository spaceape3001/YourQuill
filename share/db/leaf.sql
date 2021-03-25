
CREATE TABLE Leafs (
                -- This ID will be the corresponding document ID
	id			INTEGER PRIMARY KEY,
	k			VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
	atom        INTEGER DEFAULT 0,
	icon        INTEGER,
	title		VARCHAR(255) COLLATE NOCASE
);

