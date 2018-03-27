#include <cstring>  
#include <cstdlib>  
#include <iostream>  
#include <zlib.h>

#include <baidugz.h>

#include "gzip.h"

int main() {  
  int err = 0;  
  Byte compr[200], uncompr[200];  // big enough
  uLong comprLen, uncomprLen;

  char* hello = "12345678901234567890123456789012345678901234567890";
  uLong len = strlen(hello) + 1;
  comprLen  = sizeof(compr) / sizeof(compr[0]);
  uncomprLen  = sizeof(uncompr) / sizeof(uncompr[0]);

  err = gzcompress((Bytef*)hello, len, compr, &comprLen);
  if (err != Z_OK) {
    std::cerr << "compess error: " << err << '\n';
    return -1;
  }
  std::cout << "orignal size: " << len
            << " , compressed size : " << comprLen << '\n';

  err = gzdecompress(compr, comprLen, uncompr, &uncomprLen);
  if (err != Z_OK) {
    std::cerr << "uncompess error: " << err << '\n';
    return -1;
  }
  std::cout << "orignal size: " << len
            << " , uncompressed size : " << uncomprLen << '\n';

  if (strcmp((char*)uncompr, hello)) {
    std::cerr << "BAD uncompress!!!\n";
    return -1;
  } else {
    std::cout << "uncompress() succeed: \n" << (char *)uncompr << std::endl;
  }

  std::cout << "Start test baidu unzip." << std::endl;
  int out_len = sizeof(uncompr) / sizeof(uncompr[0]);
  int ret = baidu_ungzip(compr, comprLen, uncompr, &out_len, 0);
  if (ret != 0) {
    std::cerr << "baidu uncompess error: " << ret << '\n';
    return -1;
  }
  std::cout << "orignal size: " << len
            << " , uncompressed size : " << out_len << '\n';

  if (strcmp((char*)uncompr, hello)) {
    std::cerr << "BAD uncompress!!!\n";
    return -1;
  } else {
    std::cout << "uncompress() succeed: \n" << (char *)uncompr;
  }
}
