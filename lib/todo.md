# Functions
- bool addDir(const char* path)
  - False - Error
  - True - Directory added
- unsigned int getDir(const char* path)
  - -1 - Error / doesn't exist
  - (id) - Directory ID
- bool getDirName(unsigned int id, char* name)
  - False - Error
  - True - Directory found, name returned
- short fileExists(unsigned int dir, const char* filename)
  - -1 - Error
  - 0 - Does not exist
  - 1 - Exists
- bool addFile(unsigned int dir, const char* filename)
  - False - Error
  - True - File added
- unsigned int getFile(unsigned int dir, const char* filename)
  - -1 - Error / doesn't exist
  - (id) - File ID
- bool getFileName(unsigned int id, char* filename)
  - False - Error
  - True - File found, filename returned

# Database structure
- Table tag
  - For storing tags
- Table filetag
  - For linking files and tags N:N