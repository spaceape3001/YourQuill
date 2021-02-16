
    --  =============================================================
    --      TAGS
    --  =============================================================

CREATE TABLE Tags (
    id          INTEGER PRIMARY KEY,
	k			VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
	name		VARCHAR(255) COLLATE NOCASE,
	icon        INTEGER,
	leaf        INTEGER,
	brief       VARCHAR(255)
);

