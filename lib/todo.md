# Functions
- short tagExists(const char* value, char** errmsg)
  - -1 Error
  - 0 Does not exist
  - 1 Exists
- bool addTag(const char* value, char** errmsg)
  - false Error
  - true Added successfully
- int getTag(const char* value, char** errmsg)
  - -1 Error or doesn't exist
  - (ID) Tag ID
- bool getTagValue(unsigned int id, char** errmsg)
  - false Error
  - true Returned

# Database structure
- Table filetag
  - For linking files and tags N:N