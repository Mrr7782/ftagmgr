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
    short sres = ftagmgr::dirExists("/tmp/test", &err);
    if (sres == 0) {
        std::cout << "Non-existent directory check OK." << std::endl;
    } else if (sres == 1) {
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
    // Add dir
    bool bres = false;
    bres = ftagmgr::addDir("/tmp/test", &err);
    if (bres) std::cout << "Directory addition OK." << std::endl;
    else {
        std::cout << "Directory addition failed, ";
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else {
            std::cout << "FTagMgrLib error." << std::endl;
        }
    }
    // Check dir existence again
    sres = -2;
    sres = ftagmgr::dirExists("/tmp/test", &err);
    if (sres == 1) std::cout << "Existent directory check OK." << std::endl;
    else if (sres == 0) std::cout << "Existent directory check failed." << std::endl << "Existent directory reported as not existing." << std::endl;
    else {
        std::cout << "Existent directory check failed, ";
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else {
            std::cout << "FTagMgrLib error." << std::endl;
        }
    }
    // Get directory ID
    unsigned int uires = ftagmgr::getDir("/tmp/test", &err);
    if (uires == -1) {
        std::cout << "Getting the directory ID failed, ";
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else {
            std::cout << "FTagMgrLib error." << std::endl;
        }
    } else std::cout << "Getting the directory ID OK. (" << uires << ")" << std::endl;
    return 0;
}