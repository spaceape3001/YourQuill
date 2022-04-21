CREATE TABLE Images (
    --  icon IDs are the fragment IDs
    id          INTEGER PRIMARY KEY,
    type        VARCHAR(255),

        -- thumbnails if raster
    small       BLOB,
    medium      BLOB,
    large       BLOB
);


CREATE TABLE Graphs (
    id          INTEGER PRIMARY KEY,
    name        VARCHAR(255),
    dot         TEXT,
    html        TEXT,
    svg         BLOB
);

CREATE TABLE RootIcons (
    root        PRIMARY KEY,
    icon        INTEGER
);

