	--	Files is used for tracking whether a file is new/modified
CREATE TABLE Fragments (
		-- auto-increment is fine (avoids issues)
	id			INTEGER PRIMARY KEY,
	path		VARCHAR(255) NOT NULL UNIQUE,

	    -- full filename (no path)
	name		VARCHAR(255) NOT NULL, 
	
	    -- file extension
	suffix      VARCHAR(255) COLLATE NOCASE,

        -- Location/linkage
	dir         INTEGER NOT NULL,
	root		INTEGER NOT NULL,
	document    INTEGER NOT NULL,
	folder      INTEGER NOT NULL,
	
        -- General info
	bytes    	INTEGER NOT NULL DEFAULT 0,
	modified	INTEGER NOT NULL DEFAULT 0,
	removed     BOOLEAN NOT NULL DEFAULT 0,
    hidden      BOOLEAN NOT NULL DEFAULT 0,
	rescan      BOOLEAN NOT NULL DEFAULT 0
);

CREATE TABLE Directories (
		-- auto-increment is fine (avoids issues)
	id			INTEGER PRIMARY KEY,
	path        VARCHAR(255) NOT NULL UNIQUE,
	name        VARCHAR(255),
	root        INTEGER NOT NULL,
	folder      INTEGER NOT NULL,
	removed     BOOLEAN NOT NULL DEFAULT 0,
    hidden      BOOLEAN NOT NULL DEFAULT 0,
	parent      INTEGER
);



--  A file resolving (case insensitive) in user-space
CREATE TABLE Documents (
    id          INTEGER PRIMARY KEY,
	k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
	sk          VARCHAR(255) NOT NULL COLLATE NOCASE,
	name        VARCHAR(255) COLLATE NOCASE,
	base        VARCHAR(255) COLLATE NOCASE,
	folder      INTEGER,
	icon        INTEGER,
	suffix      VARCHAR(255),
    hidden      BOOLEAN NOT NULL DEFAULT 0,
	removed     BOOLEAN NOT NULL DEFAULT 0
);

-- A folder resolves directories in case-insensitive user-space
CREATE TABLE Folders (
	id			INTEGER PRIMARY KEY,
	k           VARCHAR(255) NOT NULL UNIQUE COLLATE NOCASE,
	sk          VARCHAR(255) NOT NULL COLLATE NOCASE,
	parent      INTEGER NOT NULL DEFAULT 0,
	name        VARCHAR(255) NOT NULL,
	brief       VARCHAR(255),
	icon        INTEGER,
    hidden      BOOLEAN NOT NULL DEFAULT 0,
	removed     BOOLEAN NOT NULL DEFAULT 0
);

INSERT INTO Folders (id,k,parent,sk,name) VALUES (1, '',         0, '',         'Root');
INSERT INTO Folders (id,k,parent,sk,name) VALUES (2, '.config',  1, '.config',  'Config');
INSERT INTO Folders (id,k,parent,sk,name) VALUES (3, '.classes', 1, '.classes', 'Classes');
INSERT INTO Folders (id,k,parent,sk,name) VALUES (4, '.tags',    1, '.tags',    'Tags');



