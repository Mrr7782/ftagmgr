/**
 * @file ftagmgrlib.cpp
 * @brief FTagMgr library source code
 */

#include <string>
#include <cstring>
#include <sys/stat.h>
#include <sqlite3.h>

namespace ftagmgr {
    std::string databasePath;
    // What to do when callback is called
    enum enum_callback { CALLBACK_NULL, CALLBACK_DIRCHECK, CALLBACK_GETID, CALLBACK_GETDIRPATH };
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
     * @brief Check database file existence
     * @retval true File exists
     * @retval false File does not exist
     */
    bool checkDatabaseExistence() {
        struct stat fileStat;
        if (stat(databasePath.c_str(), &fileStat) == 0) return true;
        else return false;
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
            case CALLBACK_GETID:
                // Return argv if possible
                if (argc) *(unsigned int*)sharedVar = strtoul(argv[0], nullptr, 10);
                break;
            case CALLBACK_GETDIRPATH:
                // Copy path char* if possible
                if (argc) *(std::string*)sharedVar = std::string(argv[0]);
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
        // Check that the database file doesn't exist
        if(checkDatabaseExistence()) return false;
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
     * @param path Path of the directory to check
     * @param errmsg SQLite3 error message char**
     * @retval -1 An error has occurred
     * @retval 0 Directory does not exist
     * @retval 1 Directory does exist
     */
    short dirExists(const char* path, char** errmsg) {
        // Check database file existence
        if (!checkDatabaseExistence()) return -1;
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
        query += path;
        query += "\";";
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            return -1;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        return result ? 1 : 0;
    }

    /**
     * @brief Adds a directory to the database
     * @param path The path of the directory
     * @param errmsg SQLite3 error message char**
     * @retval true Directory added
     * @retval false Directory could not be added
     */
    bool addDir(const char* path, char** errmsg) {
        //Check directory existence
        if (!dirExists(path, errmsg)) {
            // Open database
            sqlite3* db = nullptr;
            int ecode = 0;
            sqlite3_open(databasePath.c_str(), &db);
            if (!db) return false;
            // Prepare query
            std::string query = "INSERT INTO dir(path) VALUES(\"";
            query += path;
            query += "\");";
            // Run query
            ecode = sqlite3_exec(db, query.c_str(), nullptr, nullptr, errmsg);
            if (ecode != SQLITE_OK) {
                sqlite3_close(db);
                return false;
            }
            // Close database and return
            sqlite3_close(db);
            return true;
        } else return false;
    }

    /**
     * @brief Get directory ID by path
     * @param path The directory path to search for
     * @param errmsg SQLite3 error message char**
     * @retval -1 Error or directory doesn't exist
     * @return The ID of the directory
     */
    unsigned int getDir(const char* path, char** errmsg) {
        // Check database existence
        if (!checkDatabaseExistence()) return -1;
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        if (!db) return -1;
        // Prepare callback
        unsigned int res = 0;
        callbackAction = CALLBACK_GETID;
        sharedVar = &res;
        // Prepare query
        std::string query = "SELECT id FROM dir WHERE path = \"";
        query += path;
        query += "\";";
        // Run query
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            return -1;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        return res;
    }

    /**
     * @brief Get directory path by ID
     * @param id The directory ID to search for
     * @param path Pointer to the return std::string
     * @param errmsg SQLite3 error message char**
     * @retval true Directory found, name returned
     * @retval false An error has occurred
     */
    bool getDirPath(unsigned int id, std::string* path, char** errmsg) {
        // Check database existence
        if (!checkDatabaseExistence()) return false;
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        // Prepare callback
        callbackAction = CALLBACK_GETDIRPATH;
        sharedVar = path;
        // Prepare query
        std::string query = "SELECT path FROM dir WHERE id = ";
        query += std::to_string(id);
        query += ";";
        // Run query
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            return false;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        return true;
    }
}