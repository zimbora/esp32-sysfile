
#include "sysfile.hpp"

SYSFILE sysfile;

#define MAX_DIRECTORIES 5
#define MAX_FILES 5
#define MAX_TESTS 10

String directory[] = {
  "/home/lucas",
  "/home/guest",
  "/etc",
  "/opt",
  "/opt/app.txt"
};

String files[] = {
  "/home/lucas/info.txt",
  "/home/lucas/profile.txt",
  "/home/guest/profile.txt",
  "/opt/config.txt",
  "/opt/app2"
};


bool test_res[MAX_TESTS];
String test_message[MAX_TESTS];

uint8_t test_count = 0;
void save_test_result(bool res, String msg){

  if(test_count>=MAX_TESTS)
    return;

  String test_msg = "test: "+String(test_count) + " " + msg;
  test_res[test_count] == res;
  test_message[test_count] = test_msg;
  test_count++;
}

bool isValidDirectory(const char* directory){
  std::string dir = std::string(directory);

  if(dir.find(".") != std::string::npos){
    return false;
  }
  return true;
}

bool isValidFile(const char* filename){
  std::string file = std::string(filename);

  if(file.find(".") == std::string::npos){
    return false;
  }
  return true;
}


void setup() {
  // put your setup code here, to run once:

  bool error = false;

  Serial.begin(115200);

  sysfile.init();

  sysfile.format();

  int16_t nfiles = sysfile.countFiles("/");

  if(nfiles != 0)
    save_test_result(false,"-- test has FAILED - counting files --");
  else save_test_result(true,"-- test has PASSED - counting files --");

  uint8_t i = 0;
  while(i < 4){
    if(!isValidDirectory(directory[i].c_str())){
      i++;
      continue;
    }
    if(!sysfile.create_dir(directory[i++].c_str())){
      error = true;
    }
  }
  if(error)
    save_test_result(false,"-- test has FAILED - creating directories --");
  else
    save_test_result(false,"-- test has PASSED - creating directories --");


  sysfile.list_filesystem(2);

  i = 0;
  String text = "Hello mtf, guess who is here??";
  while(i < MAX_FILES){
    if(!isValidFile(files[i].c_str())){
      i++;
      continue;
    }
    if(!sysfile.write_file(files[i++].c_str(),text.c_str(),text.length()))
      error = true;
  }

  if(error)
    save_test_result(false,"-- test has FAILED - creating and writing files --");
  else
    save_test_result(false,"-- test has PASSED - creating and writing files --");

  sysfile.list_filesystem(3);

  nfiles = sysfile.countFiles("/",2);
  if(nfiles != 6)
    save_test_result(false,"-- test has FAILED - counting files for level 2 --");
  else save_test_result(true,"-- test has PASSED - counting files for level 2 --");

  nfiles = sysfile.countFiles("/",3);
  if(nfiles != 9)
    save_test_result(false,"-- test has FAILED - counting files for level 3 --");
  else save_test_result(true,"-- test has PASSED - counting files for level 3 --");

  // reading file
  uint16_t len = 300;
  char* data = (char*)malloc(len);
  if(sysfile.read_file(files[0].c_str(),data,&len)){
    if(len != text.length())
      save_test_result(false,"-- test has FAILED - reading file --");
    else save_test_result(true,"-- test has PASSED - reading file --");
  }
  free(data);

  int16_t filesDeleted = sysfile.delete_dir("/",3);
  if(filesDeleted != 9)
    save_test_result(false,"-- test has FAILED - deleting files for level 3 --");
  else save_test_result(true,"-- test has PASSED - deleting files for level 3 --");

  nfiles = sysfile.countFiles("/",3);
  if(nfiles != 0)
    save_test_result(false,"-- test has FAILED - counting files for level 3 --");
  else save_test_result(true,"-- test has PASSED - counting files for level 3 --");

  Serial.println("\n--- tests results ---");
  i = 0;
  while(i<MAX_TESTS){
    Serial.println(test_message[i++]);
  }
  Serial.println("--- tests ended ---");
}

void loop() {
  // put your main code here, to run repeatedly:



  delay(5000);

}
