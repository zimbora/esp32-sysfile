
#ifndef SYSFILE_H
#define SYSFILE_H

//#define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
#define FORMAT_LITTLEFS_IF_FAILED true

//#define HIGH_DEBUG_SYSFILE
//#define DEBUG_SYSFILE
#define WARNING_DEBUG_SYSFILE

#include <FS.h>
#include <LittleFS.h>

class SYSFILE{
  public:
    SYSFILE(){};

    HardwareSerial* serial = &Serial;

    /*
    * Use this constructor to change debug serial port
    *
    */
    SYSFILE(HardwareSerial* serial_port){
      serial = serial_port;
    };
    /*
    * Init filesystem
    *
    * return true if filesystem has been correctly inited
    */
    bool init();
    /*
    * Formats filesystem
    *
    * return true if filesystem has been formatted
    */
    bool format();
    /*
    * Create all necessary folders until full path are completed
    *
    * @dir - directory to be created
    *
    * return true if folder exists or has been created
    */
    bool create_dir(const char* dir);
    /*
    * delete contained folders and files
    * increase level to delete its subfolders
    *
    * @dir - directory to be deleted
    * @level - depth of folder to be listed
    *
    * return number of files and folders deleted
    */
    int16_t delete_dir(const char* dir,int8_t level);
    /*
    * @dir - directory to be listed
    * @level - depth of folder to be listed
    */
    void list_filesystem(const char* dir, uint8_t level);
    /*
    * starts to list on root folder '/'
    *
    * @level - depth of folder to be listed
    */
    void list_filesystem(uint8_t level);
    /*
    * @filename - path +  file extension
    * @data - pointer to buffer
    * @len - size of buffer
    *
    * return true if file was written successfully
    */
    bool write_file(const char* filename, const char* data, uint16_t len);
    /*
    * @filename - path +  file extension
    * @data - pointer to buffer
    * @len - size of buffer (returns new length)
    *
    * return true if file was read successfully
    */
    bool read_file(const char * filename, char* data, uint16_t* len);
    /*
    * @dirname - path
    *
    * return name of next file inside folder / returns "" if no file was found
    */
    String get_next_file(const char * dirname);
    /*
    * @filename - path + filename (extensions must include a '.')
    *
    * return result
    */
    bool delete_file(const char * filename);
    /*
    * Count files and folders inside a directory
    *
    * @dir - path to directory
    * @levels - depth of folder to be analyzed
    *
    * return number of files and folders
    */
    int16_t countFiles(const char* dir, uint8_t levels=1);
    /*
    * Iterate files inside a folder and call callback function on each file found
    *
    * @dir - path to directory
    * @timeout - available time to execute operation
    * @callback - function to be executed on each iteration
    *
    * return number of iterations
    */
    uint16_t iterateDir(const char * dirname, uint32_t timeout, bool(*callback)(String));
    /*
    * Deletes all empty subdirectories inside folder
    *
    * @dir - path to directory
    * @timeout - available time to execute operation
    */
    void deleteEmptySubDirectories(fs::FS &fs, const char * dirname, uint32_t timeout);
    /*
    * get available space on filesystem
    *
    * return available space
    */
    uint32_t get_lfs_available_space();
  private:
    int16_t check_dir(const char * dir);
    void listDir(fs::FS &fs, const char * dirname, uint8_t level);
    bool create_folder(const char* folder);

};

#endif
