#include <iostream>
#include <sqlite3.h>
#include "ftagmgrlib.h"

int main() {
    char* err = nullptr;
    if (ftagmgr::createDatabase("./test.db", &err)) std::cout << "Database creation OK." << std::endl;
    else if (err) {
        std::cout << "Database creation failed, SQLite3 error." << std::endl << err << std::endl;
        sqlite3_free(err);
    } else std::cout << "Database creation failed, FTagMgrLib error." << std::endl;
    return 0;
}