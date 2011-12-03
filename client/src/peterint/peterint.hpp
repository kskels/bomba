
#ifndef _PETERINT_H_
#define _PETERINT_H_

#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>


namespace peterint {
  inline std::string encode(size_t size) {
    std::stringstream ss;
    size_t count = 0;
    while (true) {
      if (size > 127) {
        char byte = size & 0x000000000000007F; 
        if (count) {
            byte = byte | 0x80;
        }
        ss << byte;
        size = size >> 7;
      } else {
        char byte = size;
        if (count) {
            byte = byte | 0x80;
        }
        ss << byte;
        break;
      }
      count++;
    }
    std::string data = ss.str();
    std::reverse(data.begin(), data.end());
    return data;
  }
  inline bool decode(char byte, size_t* size) {
    if (byte >> 7) { 
      *size = *size + (byte + 128);   
      *size = *size << 7; 
      return true;
    }
    *size = *size + byte; 
    return false;
  }
} // namespace

#endif // _PETERINT_H_

