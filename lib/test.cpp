/**
 * @file test.cpp
 * @brief FTagMgrLib test utility source code
 */

#include <iostream>
#include "ftagmgrlib.h"

/**
 * @brief The main function
 * @return int Exit code
 */
int main() {
    // Setup
    char* err = nullptr;
    ftagmgr::setDatabasePath("./test.db");
    // Create database
    if (ftagmgr::createDatabase(&err)) std::cout << "Database creation OK." << std::endl;
    else if (err) {
        std::cout << "Database creation failed, SQLite3 error." << std::endl << err << std::endl;
        sqlite3_free(err);
        err = nullptr;
    } else std::cout << "Database creation failed, FTagMgrLib error." << std::endl;
    // Check dir existence
    short res = ftagmgr::dirExists("/tmp/test", &err);
    if (res == 0) {
        std::cout << "Non-existent directory check OK." << std::endl;
    } else if (res == 1) {
        std::cout << "Non-existent directory check failed." << std::endl << "Non-existent directory reported as existing." << std::endl;
    } else {
        std::cout << "Non-existent directory check failed, ";
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else {
            std::cout << "FTagMgrLib error." << std::endl;
        }
    }
    return 0;
}