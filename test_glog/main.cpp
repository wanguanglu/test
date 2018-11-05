/**
* @file   main.cpp
* @author wanguanglu(mail@wanguanglu.com)
* @date   2017/06/11 00:43:07
* @brief 
*  
**/

#include <glog/logging.h>

void BasicTest() {
  LOG(INFO) << "INFO log";
  LOG(WARNING) << "WARNING log";
  LOG(ERROR) << "ERROR log";
  LOG(FATAL) << "FATAL log";
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  BasicTest();

  return 0;
}
