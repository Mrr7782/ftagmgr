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
    switch (sres) {
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

    // Check file existence again
    sres = -2;
    sres = ftagmgr::fileExists(1, "test.cpp", &err);
    switch (sres) {
        case 1:
            std::cout << "Existent file check OK." << std::endl;
            break;
        case 0:
            std::cout << "Existent file check failed." << std::endl << "Existent file reported as not existing." << std::endl;
            break;
        default:
        case -1:
            std::cout << "Existent file check failed." << std::endl;
            if (err) {
                std::cout << "SQLite3 error." << std::endl << err << std::endl;
                sqlite3_free(err);
                err = nullptr;
            } else std::cout << "FTagMgrLib error." << std::endl;
            break;
    }

    // Get file ID
    ires = -1;
    ires = ftagmgr::getFile(1, "test.cpp", &err);
    if (ires == -1) {
        std::cout << "Getting the file ID failed." << std::endl;
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } std::cout << "FTagMgrLib error." << std::endl;
    } else std::cout << "Getting the file ID OK. (" << ires << ')' << std::endl;

    // Get filename
    strres = "";
    if (ftagmgr::getFileName(1, &strres, &err)) std::cout << "Getting filename OK. (\"" << strres << "\")" << std::endl;
    else {
        std::cout << "Getting filename failed." << std::endl;
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else std::cout << "FTagMgrLib error." << std::endl;
    }

    // Get filename by ID by filename cause why not
    strres = "";
    if (ftagmgr::getFileName(ftagmgr::getFile(1, "test.cpp", &err), &strres, &err)) std::cout << "Getting filename by ID by filename OK. (\"" << strres << "\")" << std::endl;
    else {
        std::cout << "Getting filename by ID by filename failed." << std::endl;
        if (err) {
            std::cout << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
        } else std::cout << "FTagMgrLib error." << std::endl;
    }

    // Non-existent tag check
    sres = -1;
    sres = ftagmgr::tagExists("sketch", &err);
    std::cout << "Non-existent tag check ";
    switch (sres) {
        default:
        case 1:
            std::cout << "failed." << std::endl << "FTagMgrLib error." << std::endl;
            break;
        case 0:
            std::cout << "OK." << std::endl;
            break;
        case -1:
            std::cout << "failed." << std::endl << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
            break;
    }

    // Add tag
    std::cout << "Tag addition ";
    if (ftagmgr::addTag("sketch", &err)) {
        std::cout << "OK." << std::endl;
    } else if (err) {
        std::cout << "failed." << std::endl << "SQLite3 error." << std::endl << err << std::endl;
        sqlite3_free(err);
        err = nullptr;
    } else std::cout << "failed." << std::endl << "FTagMgrLib error." << std::endl;

    // Existent tag check
    sres = -1;
    sres = ftagmgr::tagExists("sketch", &err);
    std::cout << "Existent tag check ";
    switch (sres) {
        default:
        case 0:
            std::cout << "failed." << std::endl << "FTagMgrLib error." << std::endl;
            break;
        case 1:
            std::cout << "OK." << std::endl;
            break;
        case -1:
            std::cout << "failed." << std::endl << "SQLite3 error." << std::endl << err << std::endl;
            sqlite3_free(err);
            err = nullptr;
            break;
    }

    // Tag ID by name check
    ires = -1;
    ires = ftagmgr::getTag("sketch", &err);
    std::cout << "Getting tag ID ";
    if (err) {
        std::cout << "failed." << std::endl << "SQLite3 error." << std::endl << err << std::endl;
        sqlite3_free(err);
        err = nullptr;
    } else if (ires == -1) {
        std::cout << "failed." << std::endl << "FTagMgrLib error." << std::endl;
    } else std::cout << "OK. (" << ires << ')' << std::endl;
    return 0;
}
