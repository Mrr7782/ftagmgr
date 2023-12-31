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
                // or file
                // or tag, whatever, basically whatever it is we were looking for was found
                if (argc) *(bool*)sharedVar = true;
                break;
            case CALLBACK_GETID:
                // Return argv if possible
                if (argc) *(int*)sharedVar = std::stoi(argv[0], nullptr, 10);
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
        // Create table tag
        ecode = sqlite3_exec(db, "CREATE TABLE tag("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "tag VARCHAR(64) UNIQUE NOT NULL);", nullptr, nullptr, errmsg);
        if (ecode != SQLITE_OK)  {
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
        std::string query = "SELECT id FROM dir WHERE path = '";
        query += path;
        query += "';";
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            callbackAction = CALLBACK_NULL;
            sharedVar = nullptr;
            return -1;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        sharedVar = nullptr;
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
            std::string query = "INSERT INTO dir(path) VALUES('";
            query += path;
            query += "');";
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
    int getDir(const char* path, char** errmsg) {
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        if (!db) return -1;
        // Prepare callback
        int res = -1;
        callbackAction = CALLBACK_GETID;
        sharedVar = &res;
        // Prepare query
        std::string query = "SELECT id FROM dir WHERE path = '";
        query += path;
        query += "';";
        // Run query
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            callbackAction = CALLBACK_NULL;
            sharedVar = nullptr;
            return -1;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        sharedVar = nullptr;
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
            callbackAction = CALLBACK_NULL;
            sharedVar = nullptr;
            return false;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        sharedVar = nullptr;
        return true;
    }

    /**
     * @brief Check the existence of a file in the database
     * @param dir Directory ID
     * @param filename Name of the file to check
     * @param errmsg SQLite error message char**
     * @retval -1 An error has occurred
     * @retval 0 File does not exist
     * @retval 1 File does exist
     */
    short fileExists(unsigned int dir, const char* filename, char** errmsg) {
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        // Prepare callback
        bool result = false;
        sharedVar = &result;
        callbackAction = CALLBACK_DIRCHECK;
        // Prepare query
        std::string query = "SELECT id FROM file WHERE dir = ";
        query += std::to_string(dir);
        query += " AND name = '";
        query += filename;
        query += "';";
        // Run query
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            callbackAction = CALLBACK_NULL;
            sharedVar = nullptr;
            return -1;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        sharedVar = nullptr;
        return result;
    }

    /**
     * @brief Adds a file to the database
     * @param dir Directory ID
     * @param filename Name of the file to add
     * @param errmsg SQLite error message char**
     * @retval true File added successfully
     * @retval false File could not be added
     */
    bool addFile(unsigned int dir, const char* filename, char** errmsg) {
        // Check file existince in database
        if (!fileExists(dir, filename, errmsg)) {
            // Open database
            sqlite3* db = nullptr;
            int ecode = 0;
            sqlite3_open(databasePath.c_str(), &db);
            // Prepare query
            std::string query = "INSERT INTO file(dir, name) VALUES(";
            query += std::to_string(dir);
            query += ", '";
            query += filename;
            query += "');";
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
     * @brief Get file ID by dir and filename
     * @param dir Directory ID
     * @param filename Name of the file
     * @param errmsg SQLite error message char**
     * @retval -1 Error or file doesn't exist
     * @return The ID of the file
     */
    int getFile(unsigned int dir, const char* filename, char** errmsg) {
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        // Prepare callback
        callbackAction = CALLBACK_GETID;
        int res = -1;
        sharedVar = &res;
        // Prepare query
        std::string query = "SELECT id FROM file WHERE dir = ";
        query += std::to_string(dir);
        query += " AND name = '";
        query += filename;
        query += "';";
        // Execute query
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            callbackAction = CALLBACK_NULL;
            sharedVar = nullptr;
            return -1;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        sharedVar = nullptr;
        return res;
    }

    /**
     * @brief Get filename by ID
     * @param id File ID
     * @param filename Pointer to the return std::string
     * @param errmsg SQLite error message char**
     * @retval true File found, filename returned
     * @retval false An error has occurred
     */
    bool getFileName(unsigned int id, std::string* filename, char** errmsg) {
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        // Prepare callback
        callbackAction = CALLBACK_GETDIRPATH;
        sharedVar = filename;
        // Prepare query
        std::string query = "SELECT name FROM file WHERE id = ";
        query += std::to_string(id);
        query += ';';
        // Execute query
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            callbackAction = CALLBACK_NULL;
            sharedVar = nullptr;
            return false;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        sharedVar = nullptr;
        return true;
    }

    /**
     * @brief Check the existence of a tag in the database
     * @param value Tag name
     * @param errmsg SQLite error message char**
     * @retval -1 Error
     * @retval 0 Tag does not exist
     * @retval 1 Tag exists
     */
    short tagExists(const char* value, char** errmsg) {
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        if (!db) return -1;
        // Prepare callback
        callbackAction = CALLBACK_DIRCHECK;
        bool result = false;
        sharedVar = &result;
        // Prepare query
        std::string query = "SELECT id FROM tag WHERE tag = '";
        query += value;
        query += "';";
        // Execute query
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            callbackAction = CALLBACK_NULL;
            sharedVar = nullptr;
            return -1;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        sharedVar = nullptr;
        return result ? 1 : 0;
    }

    /**
     * @brief Add a tag into the database
     * @param value Tag name
     * @param errmsg SQLite error message char**
     * @retval false Error
     * @retval true Added successfully
     */
    bool addTag(const char* value, char** errmsg) {
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        if (!db) return false;
        // Prepare query
        std::string query = "INSERT INTO tag(tag) VALUES('";
        query += value;
        query += "');";
        // Execute query
        ecode = sqlite3_exec(db, query.c_str(), nullptr, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            return false;
        }
        // Close database and return
        sqlite3_close(db);
        return true;
    }
    
    /**
     * @brief Gets tag ID by tag name
     * @param value Tag name
     * @param errmsg SQLite error message char**
     * @retval -1 Error or tag does not exist
     * @return Tag ID
     */
    int getTag(const char* value, char** errmsg) {
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        if (!db) return -1;
        // Prepare callback
        callbackAction = CALLBACK_GETID;
        int result = -1;
        sharedVar = &result;
        // Prepare query
        std::string query = "SELECT id FROM tag WHERE tag = '";
        query += value;
        query += "';";
        // Execute query
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            callbackAction = CALLBACK_NULL;
            sharedVar = nullptr;
            return -1;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        sharedVar = nullptr;
        return result;
    }

    /**
     * @brief Get tag value by ID
     * @param id Tag ID
     * @param value Pointer to the return std::string
     * @param errmsg SQLite error message char**
     * @retval true Value returned
     * @retval false Error
     */
    bool getTagValue(unsigned int id, std::string* value, char** errmsg) {
        // Open database
        sqlite3* db = nullptr;
        int ecode = 0;
        sqlite3_open(databasePath.c_str(), &db);
        if (!db) return false;
        // Prepare callback
        callbackAction = CALLBACK_GETDIRPATH;
        sharedVar = value;
        // Prepare query
        std::string query = "SELECT tag FROM tag WHERE id = ";
        query += std::to_string(id);
        query += ';';
        // Execute query
        ecode = sqlite3_exec(db, query.c_str(), callback, nullptr, errmsg);
        if (ecode != SQLITE_OK) {
            sqlite3_close(db);
            callbackAction = CALLBACK_NULL;
            sharedVar = nullptr;
            return false;
        }
        // Close database and return
        sqlite3_close(db);
        callbackAction = CALLBACK_NULL;
        sharedVar = nullptr;
        return true;
    }
}
