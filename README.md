# esp32-sysfile

## Description
package to create, list and delete folder and files. It uses LITTLEFS

## Implementation
The library will create a hierarchy of folders and files like unix filesystem.
A folder will only be deleted if has no files inside. To delete the files inside you need to
pass the level of depth that you want to delete those files.
Folders cannot contain "." in the name and files are mandatory to contain "." in the name

## Public Methods

[SYSFILE()](#Constructor-1)

[SYSFILE(HardwareSerial* serial_port)](#Constructor-2)

[bool init()](#Init)

[bool format()](#Format)

[bool create_dir(const char* dir)](#Create-directory)

[int16_t delete_dir(const char* dir,int8_t level)](#Delete-directory)

[void list_filesystem(uint8_t level)](#List-filesystem-1)

[void list_filesystem(const char* dir, uint8_t level)](#List-filesystem-2)

[bool write_file(const char* filename, const char* data, uint16_t len)](#Write-file)

[bool read_file(const char * filename, char* data, uint16_t* len)](#Read-file)

[String get_next_file(const char ](#Get-next-file)

[bool delete_file(const char * filename)](#Delete-file)

[int16_t countFiles(const char* dir, uint8_t levels=1)](#Count-files)

[uint16_t iterateDir(const char * dirname, uint32_t timeout, bool(*callback)(String))](#Iterate-directory)

[uint32_t get_lfs_available_space()](#Get-available-space)

## Examples
  Run programs inside examples folder to check how it works
### demo
  Call all available public methods

## Unit Test with Arduino
  For now and unless LITTLEFS could be compiled with clang, this library will be tested directly with the hardware
### unitTest
  Run program "unitTest.ino" inside examples/unitTest to test library

## Public Methods - Extension

### Constructor 1
```
SYSFILE()
```

### Constructor 2

* @serial_port - serial port for debug
```
SYSFILE(HardwareSerial* serial_port)
```


### Init
* Init filesystem


* return true if filesystem has been correctly inited
```
bool init(); // tested
```

### Format
* Formats filesystem


* return true if filesystem has been formatted
```
bool format(); // tested
```

### Create directory
* Create all necessary folders until full path are completed


* @dir - directory to be created


* return true if folder exists or has been created
```
bool create_dir(const char* dir); // tested
```

### Delete directory
* directory is only deleted if no files or folders are in it's subfolders
* increase level to delete it's subfolders
*
* @dir - directory to be deleted
* @level - depth of folder to be listed
*
* return number of files and folders deleted
```
int16_t delete_dir(const char* dir,int8_t level); // tested
```

### List filesystem 1
* @dir - directory to be listed
* @level - depth of folder to be listed
```
void list_filesystem(const char* dir, uint8_t level); // tested
```

### List filesystem 2
* starts to list on root folder '/'
*
* @level - depth of folder to be listed
```
void list_filesystem(uint8_t level); // tested
```

### Write file
* @filename - path +  file extension
* @data - pointer to buffer
* @len - size of buffer
*
* return true if file was written successfully
```
bool write_file(const char* filename, const char* data, uint16_t len); // tested
```

### Read file
* @filename - path +  file extension
* @data - pointer to buffer
* @len - size of buffer (returns new length)
*
* return true if file was read successfully
```
bool read_file(const char * filename, char* data, uint16_t* len); // tested
```

### Get next file
* @dirname - path
*
* return name of next file inside folder / returns "" if no file was found
```
String get_next_file(const char * dirname);
```

### Delete file
* @filename - path + filename (extensions must include a '.')
*
* return result
```
bool delete_file(const char * filename); // tested
```

### Count files
* Count files and folders inside a directory
*  
* @dir - path to directory
* @levels - depth of folder to be analyzed
*
* return number of files and folders
```
int16_t countFiles(const char* dir, uint8_t levels=1); // tested
```

### Iterate directory
* Iterate files inside a folder and call callback function on each iteration
*
* @dir - path to directory
* @timeout - available time to execute operation
* @callback - function to be executed on each iteration
*
* return number of iterations
```
uint16_t iterateDir(const char * dirname, uint32_t timeout, bool(*callback)(String)); // tested
```

### Get available space
* get available space on filesystem
*
* return available space
```
uint32_t get_lfs_available_space();
```
