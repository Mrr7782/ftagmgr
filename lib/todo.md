# Functions
- int getTag(const char* value, char** errmsg)
  - -1 Error or doesn't exist
  - (ID) Tag ID
- bool getTagValue(unsigned int id, char** errmsg)
  - false Error
  - true Returned

# Database structure
- Table filetag
  - For linking files and tags N:N
