#include <cstring>  
#include <cstdlib>  
#include <iostream>  
#include <zlib.h>

int main() {  
  int err = 0;  
  Byte compr[200], uncompr[200];  // big enough
  uLong comprLen, uncomprLen;

  const char* hello = "12345678901234567890123456789012345678901234567890";
  uLong len = strlen(hello) + 1;
  comprLen  = sizeof(compr) / sizeof(compr[0]);
  uncomprLen  = sizeof(uncompr) / sizeof(uncompr[0]);

  err = compress(compr, &comprLen, (const Bytef*)hello, len);
  if (err != Z_OK) {
    std::cerr << "compess error: " << err << '\n';
    return -1;
  }
  std::cout << "orignal size: " << len
            << " , compressed size : " << comprLen << '\n';

  err = uncompress(uncompr, &uncomprLen, compr, comprLen);
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
    std::cout << "uncompress() succeed: \n" << (char *)uncompr;
  }
}
