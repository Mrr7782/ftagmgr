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
    int getDir(const char* path, char** errmsg);

    /**
     * @brief Get directory path by ID
     * @param id The directory ID to search for
     * @param path Pointer to the return std::string
     * @param errmsg SQLite3 error message char**
     * @retval true Directory found, name returned
     * @retval false An error has occurred
     */
    bool getDirPath(unsigned int id, std::string* path, char** errmsg);

    /**
     * @brief Check the existence of a file in the database
     * @param dir Directory ID
     * @param filename Name of the file to check
     * @param errmsg SQLite error message char**
     * @retval -1 An error has occurred
     * @retval 0 File does not exist
     * @retval 1 File does exist
     */
    short fileExists(unsigned int dir, const char* filename, char** errmsg);

    /**
     * @brief Adds a file to the database
     * @param dir Directory ID
     * @param filename Name of the file to add
     * @param errmsg SQLite error message char**
     * @retval true File added successfully
     * @retval false File could not be added
     */
    bool addFile(unsigned int dir, const char* filename, char** errmsg);

    /**
     * @brief Get file ID by dir and filename
     * @param dir Directory ID
     * @param filename Name of the file
     * @param errmsg SQLite error message char**
     * @retval -1 Error or file doesn't exist
     * @return The ID of the file
     */
    int getFile(unsigned int dir, const char* filename, char** errmsg);

    /**
     * @brief Get filename by ID
     * @param id File ID
     * @param filename Pointer to the return std::string
     * @param errmsg SQLite error message char**
     * @retval true File found, filename returned
     * @retval false An error has occurred
     */
    bool getFileName(unsigned int id, std::string* filename, char** errmsg);

    /**
     * @brief Check the existence of a tag in the database
     * @param value Tag name
     * @param errmsg SQLite error message char**
     * @retval -1 Error
     * @retval 0 Tag does not exist
     * @retval 1 Tag exists
     */
    short tagExists(const char* value, char** errmsg);

    /**
     * @brief Add a tag into the database
     * @param value Tag name
     * @param errmsg SQLite error message char**
     * @retval false Error
     * @retval true Added successfully
     */
    bool addTag(const char* value, char** errmsg);

    /**
     * @brief Gets tag ID by tag name
     * @param value Tag name
     * @param errmsg SQLite error message char**
     * @retval -1 Error or tag does not exist
     * @return Tag ID
     */
    int getTag(const char* value, char** errmsg);

    /**
     * @brief Get tag value by ID
     * @param id Tag ID
     * @param value Pointer to the return std::string
     * @param errmsg SQLite error message char**
     * @retval true Value returned
     * @retval false Error
     */
    bool getTagValue(unsigned int id, std::string* value, char** errmsg);
}
