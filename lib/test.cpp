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
    if (sres == 0) std::cout << "Non-existent directory check OK." << std::endl;
    else if (sres == 1) std::cout << "Non-existent directory check failed." << std::endl << "Non-existent directory reported as existing." << std::endl;
    else {
        std::cout << "Non-existent directory check failed, ";
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else std::cout << "FTagMgrLib error." << std::endl;
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
        } else std::cout << "FTagMgrLib error." << std::endl;
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
        } else std::cout << "FTagMgrLib error." << std::endl;
    }

    // Get directory ID
    int ires = ftagmgr::getDir("/tmp/test", &err);
    if (ires == -1) {
        std::cout << "Getting the directory ID failed, ";
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else std::cout << "FTagMgrLib error." << std::endl;
    } else std::cout << "Getting the directory ID OK. (" << ires << ")" << std::endl;

    // Get directory path
    std::string strres = "";
    if (ftagmgr::getDirPath(1, &strres, &err)) std::cout << "Getting the directory path OK. (\"" << strres << "\")" << std::endl;
    else {
        std::cout << "Getting the directory path failed, ";
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else std::cout << "FTagMgrLib error." << std::endl;
    }

    // Get directory path by ID by path
    strres = "";
    // Writing code like this is a bad idea, but who cares
    // this is just a test program anyway
    if (ftagmgr::getDirPath(ftagmgr::getDir("/tmp/test", &err), &strres, &err)) std::cout << "Getting the directory path by its ID by its path OK. (\"" << strres << "\")" << std::endl;
    else {
        std::cout << "Getting the directory path by its ID by its path failed, ";
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else std::cout << "FTagMgrLib error." << std::endl;
    }

    // Check file existence
    sres = -2;
    sres = ftagmgr::fileExists(1, "test.cpp", &err);
    switch(sres) {
        case 1:
            std::cout << "Non-existent file check failed." << std::endl << "Non-existent file reported as existing." << std::endl;
            break;
        case 0:
            std::cout << "Non-existent file check OK." << std::endl;
            break;
        default:
        case -1:
            std::cout << "Non-existent file check failed." << std::endl;
            if (err) {
                std::cout << "SQLite3 error." << std::endl << err << std::endl;
                sqlite3_free(err);
                err = nullptr;
            } else std::cout << "FTagMgrLib error." << std::endl;
            break;
    }

    // Add file
    if (ftagmgr::addFile(1, "test.cpp", &err)) {
        std::cout << "File addition OK." << std::endl;
    } else {
        std::cout << "File addition failed." << std::endl;
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else std::cout << "FTagMgrLib error." << std::endl;
    }
    return 0;
}