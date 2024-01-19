CREATE TABLE Images (
    --  icon IDs are the fragment IDs
    id          INTEGER PRIMARY KEY,
    type        VARCHAR(255),
    width       INTEGER DEFAULT 0,
    height      INTEGER DEFAULT 0,

        -- thumbnails if raster
    small       BLOB,
    medium      BLOB,
    large       BLOB
);


