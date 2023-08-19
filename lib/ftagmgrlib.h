/**
 * @file ftagmgrlib.h
 * @brief FTagMgrLib header file
 */

#include <string>
#include <sqlite3.h>

namespace ftagmgr {
    /**
     * @brief Set the database path string
     * @param path Path to the database file
     */
    void setDatabasePath(const char* path);

    /**
     * @brief Create a new database file
     * @param errmsg SQLite3 error message char**
     * @retval true Database was created successfully
     * @retval false Database creation failed - check for file existence and/or write access to directory
     */
    bool createDatabase(char** errmsg);

    /**
     * @brief Checks the existence of a directory in the database
     * @param path Path of the directory to check
     * @param errmsg SQLite3 error message char**
     * @retval -1 An error has occurred
     * @retval 0 Directory does not exist
     * @retval 1 Directory does exist
     */
    short dirExists(const char* path, char** errmsg);

    /**
     * @brief Adds a directory to the database
     * @param path The path of the directory
     * @param errmsg SQLite3 error message char**
     * @retval true Directory added
     * @retval false Directory could not be added
     */
    bool addDir(const char* path, char** errmsg);

    /**
     * @brief Get directory ID by path
     * @param path The directory path to search for
     * @param errmsg SQLite3 error message char**
     * @retval -1 Error or directory doesn't exist
     * @return The ID of the directory
     */
    unsigned int getDir(const char* path, char** errmsg);
}