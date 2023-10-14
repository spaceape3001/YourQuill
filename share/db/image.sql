CREATE TABLE Images (
    --  icon IDs are the fragment IDs
    id          INTEGER PRIMARY KEY,
    type        VARCHAR(255),

        -- thumbnails if raster
    small       BLOB,
    medium      BLOB,
    large       BLOB
);


