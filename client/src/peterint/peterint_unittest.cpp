
#include <peterint.hpp>
#include <gtest/gtest.h>


TEST(PeterintTest, TestEncodeDecode) 
{

  std::string data = peterint::encode(111111111);
  size_t size = 0, count = 1; 
  bool more = true; 
  while(more) {
    more = peterint::decode(data[data.size()-count], &size);
    ++count;  
  }
  std::cout << "Result: " << size << std::endl; 
 

  std::cout << "------------------------------------------------------" << std::endl;

  char byte = 255;
  size = 0;

  // check the last bit
  bool bit = byte >> 7;
  std::cout << "bit: " << bit << std::endl;

  EXPECT_TRUE(peterint::decode(byte, &size));
  std::cout << size << std::endl;

  byte = 100;
  EXPECT_FALSE(peterint::decode(byte, &size));
  std::cout << size << std::endl;
}
     
