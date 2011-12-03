
#ifndef _PETERINT_H_
#define _PETERINT_H_

#include <string>
#include <iostream>


namespace peterint {
  inline std::string encode(size_t size) {
    return "Kaspars";
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
};

#endif // _PETERINT_H_

