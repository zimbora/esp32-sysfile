#include "sysfile.hpp"
#include "string.h"


bool SYSFILE::init(){

  if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    #ifdef DEBUG_SYSFILE
      Serial.println("LittleFS Mount Failed");
    #endif
    return false;
  }
  return true;
}

bool SYSFILE::format(){

  return LittleFS.format();
}

bool SYSFILE::create_dir(const char* dir){

  std::string dirname = std::string(dir);
  if(dirname.find(".") != std::string::npos){
    #ifdef WARNING_DEBUG_SYSFILE
      Serial.println("directory is invalid");
    #endif
    return false;
  }

  #ifdef DEBUG_SYSFILE
  Serial.printf("creating dir: %s \n",dir);
  #endif

  std::string full_path = std::string(dir);
  std::string path = "";
  while(true){

    std::string::size_type last_index = full_path.find("/",1);
    path += full_path.substr(0,last_index);

    #ifdef DEBUG_SYSFILE
    Serial.printf("create folder: %s \n",path.c_str());
    #endif
    if(!create_folder(path.c_str())){
      #ifdef DEBUG_SYSFILE
      Serial.printf("creating dir: %s has failed !! \n",path.c_str());
      #endif
      return false;
    }

    if(last_index == std::string::npos)
      break;

    full_path = full_path.substr(last_index);

  }

  return true;
}

int16_t SYSFILE::delete_dir(const char* dir, int8_t level){

  uint16_t counter = 0;


  #ifdef DEBUG_SYSFILE
  Serial.printf("deleting dir: %s \n",dir);
  #endif

  if(!LittleFS.rmdir(dir)){
    //Serial.printf("- failed to delete directory: %s \n",dir);

    if(level == 0)
    return counter;

    File root = LittleFS.open(dir);

    if(!root){
      #ifdef DEBUG_SYSFILE
      Serial.println("[del dir] - failed to open directory, try delete file");
      #endif
      return counter;
    }

    int16_t n_files = countFiles(dir,1);
    if(n_files <= 0){
      #ifdef DEBUG_SYSFILE
      Serial.printf("No files found in directory: %s \n",dir);
      #endif
      return 0;
    }

    File file = root.openNextFile();
    while(file){
      if(!file.isDirectory()){
        String filename = file.path();
        file.close(); // close file before try to delete
        if(delete_file(filename.c_str())){
          counter++;
        }
      }else{
        counter += delete_dir(file.path(),level - 1);
      }
      file = root.openNextFile();
    }
    file.close();
    counter += delete_dir(dir,level - 1);

    root.close();
  }else return 1;

  return counter;
}

bool SYSFILE::write_file(const char* filename, const char* data, uint16_t length){

  bool res = false;
  File file;

  std::string dir = std::string(filename);
  if(dir.find(".") == std::string::npos){
    #ifdef WARNING_DEBUG_SYSFILE
      Serial.println("filename has wrong extension");
    #endif
    return res;
  }

  if(LittleFS.usedBytes() + length >= LittleFS.totalBytes()){
    #ifdef DEBUG_SYSFILE
    Serial.println("Filesystem went out of capacity");
    #endif
    return false;
  }

  #ifdef DEBUG_SYSFILE
  Serial.println("file: "+String(filename)+" - writing "+String(length)+" bytes");
  #endif

  /*
  if (LittleFS.exists(filename))
    LittleFS.remove(filename);
  */

  file = LittleFS.open(filename, FILE_WRITE);

  if (!file){
    #ifdef DEBUG_SYSFILE
    Serial.println("- failed to open file for writing");
    #endif
    return false;
  }

  #ifdef DEBUG_SYSFILE
  Serial.println("creating new file");
  #endif

  uint32_t size = file.write((const uint8_t*)data,length);

  if(size == length){
    #ifdef DEBUG_SYSFILE
    Serial.println("file "+ String(filename) +" successfully written");
    #endif
    res = true;
  }else{
    #ifdef DEBUG_SYSFILE
    Serial.printf("Only %d bytes were written of a total of %d \n",size,length);
    #endif
  }


  file.close();

  return res;
}

bool SYSFILE::read_file(const char * filename, char* data, uint16_t* len){
  //Serial.println("reading directory: " + String(filename));

  File file;

  if (!LittleFS.exists(filename))
    return false;

  file = LittleFS.open(filename);

  if(file.isDirectory()){
    file.close();
    return false;
  }

  if(file.size() >= (uint16_t)0xFFFF){
    file.close();
    return false;
  }

  for (uint16_t i = 0; i < file.size(); i++) {
    data[i] = file.read();
  }

  *len = file.size();

  file.close();
  return true;
}

bool SYSFILE::delete_file(const char * filename){

  bool res = false;

  std::string dir = std::string(filename);
  if(dir.find(".") == std::string::npos){
    #ifdef WARNING_DEBUG_SYSFILE
      Serial.println("filename has wrong extension");
    #endif
    return res;
  }

  #ifdef DEBUG_SYSFILE
  Serial.printf("deleting file: %s \n",filename);
  #endif

  if (LittleFS.exists(filename)){
    res = LittleFS.remove(filename);
  }

  return res;
}

void SYSFILE::list_filesystem(uint8_t level){

  listDir(LittleFS,"/",level);
}

void SYSFILE::list_filesystem(const char* dir, uint8_t level){

  listDir(LittleFS,dir,level);
}

int16_t SYSFILE::countFiles(const char* dirname, uint8_t levels){

  int16_t count = 0;

  if(levels==0)
    return count;

  if(!LittleFS.exists(dirname)){
    #ifdef DEBUG_SYSFILE
      Serial.printf("[listDir] - path %s doesn't exist \n",dirname);
    #endif
    return count;
  }

  File root = LittleFS.open(dirname);
  if(!root){
    #ifdef HIGH_DEBUG_SYSFILE
    Serial.println("- Invalid path");
    #endif
    return count;
  }
  if(!root.isDirectory()){
    #ifdef HIGH_DEBUG_SYSFILE
    Serial.println(" - not a directory");
    #endif
    return count;
  }

  File file = root.openNextFile();
  while(file){
    #ifdef HIGH_DEBUG_SYSFILE
    Serial.printf("filename: %s \n",file.name());
    #endif
    count++;
    if(file.isDirectory()){
      if(levels){
          count += countFiles(file.path(), levels - 1);
      }
    }
    file = root.openNextFile();
  }
  return count;
}

String SYSFILE::get_next_file(const char * dirname){

  //Serial.printf("Listing directory: %s\r\n", dirname);
  String filename = "";
  File root = LittleFS.open(dirname);

  if(!root){
    Serial.println("- failed to open directory");
    return filename;
  }
  if(!root.isDirectory()){
    Serial.println(" - not a directory");
    return filename;
  }

  File file = root.openNextFile();
  while(file){
    if(!file.isDirectory()){
      filename = file.name();
      break;
    }
    file = root.openNextFile();
  }
  file.close();
  return filename;
}

uint16_t SYSFILE::iterateDir(const char * dirname, uint32_t timeout, bool(*callback)(String dirname)){

  //Serial.printf("Listing directory: %s\r\n", dirname);
  uint16_t count = 0;
  File root;
  root = LittleFS.open(dirname);

  if(!root){
    Serial.println("- failed to open directory");
    return count;
  }
  if(!root.isDirectory()){
    Serial.println(" - not a directory");
    return count;
  }

  File file = root.openNextFile();
  while(file && timeout > millis()){
    if(file.isDirectory()){
      iterateDir(file.name(), timeout,callback);
    } else {
      String filename = file.name();
      file.close();
      count += callback(filename);
    }
    file = root.openNextFile();
  }
  file.close();

  return count;
}

void SYSFILE::deleteEmptySubDirectories(fs::FS &fs, const char * dirname, uint32_t timeout){

  uint16_t count = 0;

  String path = String(dirname);
  String subpath = "";
  int8_t index = path.indexOf("/",1);
  if(index != -1){
    subpath = path.substring(0,index);
    path = path.substring(index);
  }else subpath = path;

  subpath = String(dirname);
  File root = fs.open(subpath.c_str()); // open main directory

  if(!root){
    Serial.println("- failed to open directory: "+String(dirname));
    return;
  }
  if(!root.isDirectory()){
    Serial.println(" - not a directory: "+String(dirname));
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      if(countFiles(file.name(),1) == 0){
        if(!fs.rmdir(file.name()))
          Serial.printf("- failed to delete directory: %s \n",file.name());
      }else{
        deleteEmptySubDirectories(fs, file.name(), timeout);
        if(countFiles(file.name(),1) == 0){
          if(!fs.rmdir(file.name()))
            Serial.printf("- failed to delete directory: %s \n",file.name());
        }
      }
    }
    file = root.openNextFile();
  }

  root.close();

  return;
}

uint32_t SYSFILE::get_lfs_available_space(){
  uint32_t available_space = (LittleFS.totalBytes() - LittleFS.usedBytes())/1000;
  return available_space;
}

// return number of subfolders or files inside the directory
int16_t SYSFILE::check_dir(const char * dirname){

  int16_t counter = 0;
  File root;

  root = LittleFS.open(dirname);
  if(!root){
    return counter;
  }

  counter = 0;
  File file = root.openNextFile();
  while(file){
    if(!file.isDirectory())
    counter++;

    file = root.openNextFile();
  }

  file.close();

  return counter;
}

// print directories
void SYSFILE::listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    //Serial.printf("Listing directory: %s\r\n", dirname);
    if(levels == 0)
      return;

    if(!fs.exists(dirname)){
      #ifdef DEBUG_SYSFILE
        Serial.printf("[listDir] - path %s doesn't exist \n",dirname);
      #endif
      return;
    }

    File root = fs.open(dirname);
    if(!root){
      #ifdef DEBUG_SYSFILE
        Serial.printf("[listDir] - failed to open %s directory",dirname);
      #endif
      return;
    }
    if(!root.isDirectory()){
      #ifdef DEBUG_SYSFILE
        Serial.printf("[listDir] - %s not a directory",dirname);
      #endif
      return;
    }

    File file = root.openNextFile();
    while(file){
      if(file.isDirectory()){
        Serial.print("[listDir]  DIR : ");
        Serial.println(file.name());
        if(levels){
            listDir(fs, file.path(), levels -1);
        }
      } else {
        Serial.print("[listDir]  FILE: ");
        Serial.print(file.name());
        Serial.print("\tSIZE: ");
        Serial.println(file.size());
      }
      file = root.openNextFile();
    }
    root.close();
}

bool SYSFILE::create_folder(const char* folder){

  if(folder == "")
   return false;

  if(check_dir(folder) <= 0){
    #ifdef HIGH_DEBUG_SYSFILE
      Serial.printf("creating folder: %s \n",folder);
    #endif
    if(LittleFS.mkdir(folder)){
      #ifdef HIGH_DEBUG_SYSFILE
        Serial.println("folder created");
      #endif
    }else{
      #ifdef WARNING_DEBUG_SYSFILE
        Serial.println("creating folder has failed !!");
      #endif
      return false;
    }
  }else{
   #ifdef HIGH_DEBUG_SYSFILE
    Serial.printf("folder already exists: %s \n",folder);
   #endif
  }
  return true;
}
