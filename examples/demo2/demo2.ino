
/*
* Program to delete empty subdirectories
*/

#include "sysfile.hpp"

SYSFILE sysfile;

#define NUMITEMS(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

String directory[] = {
  "/home/lucas/records/1/voltage",
  "/home/lucas/records/2/voltage",
  "/home/lucas/records/2/temp",
  "/home/lucas/records/3/temp",
  "/home/guest",
  "/opt"
};

String files[] = {
  //"/home/lucas/records/3/temp/profile.txt",
  "/home/guest/profile.txt",
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
  while(i < NUMITEMS(directory)){
    if(!sysfile.create_dir(directory[i++].c_str()))
      Serial.println("-- create dir has FAILED --");
  }

  i = 0;
  String text = "Hello mtf, guess who is here??";
  while(i < NUMITEMS(files)){
    if(!sysfile.write_file(files[i++].c_str(),text.c_str(),text.length()))
      Serial.println("-- create file has FAILED --");
  }

  Serial.printf("LFS available space (kB): %d \n",sysfile.get_lfs_available_space());

  sysfile.list_filesystem(5);

  sysfile.deleteEmptySubDirectories("/home",2000);

  Serial.printf("LFS available space (kB): %d \n",sysfile.get_lfs_available_space());

  sysfile.list_filesystem(5);
}

uint8_t loop_counter = 1;
void loop() {
  // put your main code here, to run repeatedly:



  delay(5000);

}
