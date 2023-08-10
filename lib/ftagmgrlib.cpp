/**
 * @file ftagmgrlib.cpp
 * @brief FTagMgr library source code
 */

#include <sys/stat.h>
#include <sqlite3.h>

namespace ftagmgr {

    /**
     * @brief Create a new database file
     * @param path The path where the new database file should be created
     * @param errmsg SQLite3 error message char**
     * @retval true Database was created successfully
     * @retval false Database creation failed - check for file existence and/or write access to directory
     */
    bool createDatabase(const char* path, char** errmsg) {
        // Check that file at path doesn't exist
        struct stat fileStat;
        if (stat(path, &fileStat) == 0) return false;
        // Open (create) database, check if it's open
        sqlite3* db = nullptr;
        int ecode = 0; //Exit code
        sqlite3_open(path, &db);
        if (!db) return false;
        // Create table dir
        ecode = sqlite3_exec(db, "CREATE TABLE dir("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "path VARCHAR(256) UNIQUE NOT NULL);", nullptr, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            return false;
        }
        // Create table file
        ecode = sqlite3_exec(db, "CREATE TABLE file("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "dir INTEGER NOT NULL, "
                                 "name VARCHAR(64) NOT NULL, "
                                 "FOREIGN KEY (dir) REFERENCES dir(id));", nullptr, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            return false;
        }
        // Close db and return true
        sqlite3_close(db);
        return true;
    }
}