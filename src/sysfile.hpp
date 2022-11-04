
#ifndef SYSFILE_H
#define SYSFILE_H

//#define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
#define FORMAT_LITTLEFS_IF_FAILED true

//#define DEBUG_SYSFILE
//#define HIGH_DEBUG_SYSFILE
#define WARNING_DEBUG_SYSFILE

#include <FS.h>
#include <LITTLEFS.h>

#define MAX_AR_FILE_SIZE 220

// !! Maybe filename has to be equal or greater than 8 chars, bcs file /alarm without the "_" in the end doesn't work
#define SETTINGS_FILE "/settings.txt" // settings
#define RECORDS_PATH "/records"
#define MQTT_RECORDS_PATH "/ee"
#define CALIB_ENERGY_FILE "/calib.txt"
#define PULSE_COUNTERS_PATH "/counters.txt"
#define IT_TABLE_PATH "/it_table.txt"

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
    bool init(); // tested
    /*
    * Formats filesystem
    *
    * return true if filesystem has been formatted
    */
    bool format(); // tested
    /*
    * Create all necessary folders until full path are completed
    *
    * @dir - directory to be created
    *
    * return true if folder exists or has been created
    */
    bool create_dir(const char* dir); // tested
    /*
    * directory is only deleted if no files or folders are in it's subfolders
    * increase level to delete it's subfolders
    *
    * @dir - directory to be deleted
    * @level - depth of folder to be listed
    *
    * return number of files and folders deleted
    */
    int16_t delete_dir(const char* dir,int8_t level); // tested
    /*
    * @dir - directory to be listed
    * @level - depth of folder to be listed
    */
    void list_filesystem(const char* dir, uint8_t level); // tested
    /*
    * starts to list on root folder '/'
    *
    * @level - depth of folder to be listed
    */
    void list_filesystem(uint8_t level); // tested
    /*
    * @filename - path +  file extension
    * @data - pointer to buffer
    * @len - size of buffer
    *
    * return true if file was written successfully
    */
    bool write_file(const char* filename, const char* data, uint16_t len); // tested
    /*
    * @filename - path +  file extension
    * @data - pointer to buffer
    * @len - size of buffer (returns new length)
    *
    * return true if file was read successfully
    */
    bool read_file(const char * filename, char* data, uint16_t* len); // tested
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
    bool delete_file(const char * filename); // tested
    /*
    * Count files and folders inside a directory
    *
    * @dir - path to directory
    * @levels - depth of folder to be analyzed
    *
    * return number of files and folders
    */
    int16_t countFiles(const char* dir, uint8_t levels=1); // tested
    /*
    * Iterate files inside a folder and call callback function on each iteration
    *
    * @dir - path to directory
    * @timeout - available time to execute operation
    * @callback - function to be executed on each iteration
    *
    * return number of iterations
    */
    uint16_t iterateDir(const char * dirname, uint32_t timeout, bool(*callback)(String)); // tested
    /*
    * get available space on filesystem
    *
    * return available space
    */
    uint32_t get_lfs_available_space();
  private:
    int16_t check_dir(const char * dir); // tested
    void listDir(fs::FS &fs, const char * dirname, uint8_t level); // tested
    bool create_folder(const char* folder); // tested

};

#endif
