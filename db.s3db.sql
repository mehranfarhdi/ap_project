
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

-- Table: sys_users and book

CREATE TABLE users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL UNIQUE, passwd TEXT NOT NULL, fname TEXT NOT NULL, mname TEXT, lname TEXT NOT NULL, rank INTEGER DEFAULT (1), email TEXT NOT NULL UNIQUE, UNIQUE (username))
INSERT INTO users (id, username, passwd, fname, mname, lname, rank, email) VALUES (1, 'admin', 'pass', 'Root', NULL, 'Administrator', -1, 'admin@root.org');
CREATE TABLE book (id INTEGER PRIMARY KEY AUTOINCREMENT, book_name TEXT NOT NULL UNIQUE, writer TEXT, year_of_publication TEXT, category TEXT, imagedir TEXT)
COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
