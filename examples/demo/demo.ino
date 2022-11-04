
#include "sysfile.hpp"

SYSFILE sysfile;

#define MAX_DIRECTORIES 4
#define MAX_FILES 5

String directory[] = {
  "/home/lucas",
  "/home/guest",
  "/etc",
  "/opt"
};

String files[] = {
  "/home/lucas/info.txt",
  "/home/lucas/profile.txt",
  "/home/guest/profile.txt",
  "/etc",
  "/opt/config.txt"
};

bool (*callback)(String);
bool callOnFolderIteration(String filename){
  Serial.printf("callback filename: %s \n",filename.c_str());
  return true;
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  sysfile.init();

  sysfile.format();

  sysfile.list_filesystem(1);

  uint8_t i = 0;
  while(i < MAX_DIRECTORIES){
    if(!sysfile.create_dir(directory[i++].c_str()))
      Serial.println("-- create dir has FAILED --");
  }

  i = 0;
  String text = "Hello mtf, guess who is here??";
  while(i < MAX_FILES){
    if(!sysfile.write_file(files[i++].c_str(),text.c_str(),text.length()))
      Serial.println("-- create file has FAILED --");
  }

  sysfile.list_filesystem(3);

  i = 0;
  while(i < MAX_FILES){
    uint16_t len = 300;
    char* data = (char*)malloc(len);
    memset(data,0,len);
    Serial.printf("file: %s \n",files[i].c_str());
    if(sysfile.read_file(files[i++].c_str(),data,&len)){
      uint16_t j = 0;
      while(j<len){
        Serial.print(data[j++]);
      }
      Serial.println("");
    }
    free(data);
  }

  Serial.printf("LFS available space (kB): %d \n",sysfile.get_lfs_available_space());

  String file = sysfile.get_next_file("/home/lucas");
  Serial.printf("next file: %s \n",file.c_str());
  if(file != ""){
    if(sysfile.delete_file(file.c_str()))
      Serial.printf("file: %s deleted \n",file.c_str());
  }
  file = sysfile.get_next_file("/home/lucas");
  Serial.printf("next file: %s \n",file.c_str());

  Serial.printf("LFS available space (kB): %d \n",sysfile.get_lfs_available_space());

  callback = &callOnFolderIteration;
  sysfile.iterateDir(directory[0].c_str(),5000,callback);

  int16_t filesDeleted = sysfile.delete_dir("/",3);
  Serial.printf("Files deleted: %d \n",filesDeleted);

  sysfile.list_filesystem(3);

  Serial.printf("LFS available space (kB): %d \n",sysfile.get_lfs_available_space());
}

uint8_t loop_counter = 1;
void loop() {
  // put your main code here, to run repeatedly:



  delay(5000);

}
