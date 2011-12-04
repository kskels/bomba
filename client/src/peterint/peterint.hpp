
#ifndef _PETERINT_H_
#define _PETERINT_H_

#include <string>
#include <sstream>
#include <algorithm>


// TODO: Describe Peter's integer handling algorithm, otherwise
// nobody will ever understand how this workd :D

namespace peterint {
  inline std::string encode(size_t size) {
    std::string result;
    while (true) {
      char byte = size & 0x7F;
      if (!result.empty())
        byte = byte | 0x80;
      result.insert(result.begin(), byte);

      if (size > 127)
        size = size >> 7; 
      else 
        break;
    }
    return result;
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

