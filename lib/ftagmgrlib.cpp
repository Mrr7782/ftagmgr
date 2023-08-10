/**
 * @file ftagmgrlib.cpp
 * @brief FTagMgr library source code
 */

#include <string>
#include <sys/stat.h>
#include <sqlite3.h>

namespace ftagmgr {
    std::string databasePath;
    // What to do when callback is called
    enum enum_callback { CALLBACK_NULL, CALLBACK_DIRCHECK };
    enum_callback callbackAction = CALLBACK_NULL;
    // Used for callback
    void* sharedVar = nullptr;

    
    /**
     * @brief Set the database path string
     * @param path Path to the database file
     */
    void setDatabasePath(const char* path) {
        databasePath = path;
    }

    /**
     * @brief SQLite3 callback function
     * @param unused Unused
     * @param argc How many items were returned
     * @param argv Returned values
     * @param azColName Column name of the returned values, matching index
     * @return int No idea what this is
     */
    int callback(void* unused, int argc, char** argv, char** azColName) {
        switch (callbackAction) {
            default:
            case CALLBACK_NULL:
                break;
            case CALLBACK_DIRCHECK:
                // If something was returned, the dir was found
                if (argc) *(bool*)sharedVar = true;
                break;
        }
        return 0;
    }

    /**
     * @brief Create a new database file
     * @param errmsg SQLite3 error message char**
     * @retval true Database was created successfully
     * @retval false Database creation failed - check for file existence and/or write access to directory
     */
    bool createDatabase(char** errmsg) {
        // Check that file at path doesn't exist
        struct stat fileStat;
        if (stat(databasePath.c_str(), &fileStat) == 0) return false;
        // Open (create) database, check if it's open
        sqlite3* db = nullptr;
        int ecode = 0; //Exit code
        sqlite3_open(databasePath.c_str(), &db);
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

    /**
     * @brief Checks the existence of a directory in the database
     * @param dirPath Path of the directory to check
     * @param errmsg SQLite3 error message char**
     * @retval -1 An error has occurred
     * @retval 0 Directory does not exist
     * @retval 1 Directory does exist
     */
    short dirExists(const char* dirPath, char** errmsg) {
        // Check database file existence
        struct stat fileStat;
        if (stat(databasePath.c_str(), &fileStat)) return -1;
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        if (!db) {
            return -1;
        }
        // Prepare callback
        bool result = false;
        callbackAction = CALLBACK_DIRCHECK;
        sharedVar = &result;
        // Run query
        std::string query = "SELECT id FROM dir WHERE path = \"";
        query += dirPath;
        query += "\";";
        ecode = sqlite3_exec(db, query.c_str(), nullptr, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            return -1;
        }
        // Close database and return
        sqlite3_close(db);
        return result ? 1 : 0;
    }
}