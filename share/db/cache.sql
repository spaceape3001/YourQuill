

CREATE TABLE ClsFolders (
    classId     INTEGER NOT NULL,
    folder      VARCHAR(255) COLLATE NOCASE,
	UNIQUE(classId,folder) ON CONFLICT IGNORE
);

    

    --  =============================================================
    --      IMAGES
    --  =============================================================


