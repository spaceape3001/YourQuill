CREATE TABLE Properties (
    id      INTEGER PRIMARY KEY,
    atom    INTEGER NOT NULL,
    uid     VARCHAR(255),
    k       VARCHAR(255),
    value   VARCHAR(255),
    kind    INTEGER,
    type    VARCHAR(255),
        -- resolved ID (for the kind)
    rid     INTEGER DEFAULT 0
);

CREATE TABLE PAttribute (
    prop    INTEGER NOT NULL,
    attr    INTEGER NOT NULL,
    implied BOOLEAN,
    UNIQUE(prop,attr)
);

