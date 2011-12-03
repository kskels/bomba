
#include <peterint.hpp>
#include <gtest/gtest.h>


TEST(PeterintTest, Playground) 
{

  std::cout << "------------------------------------------------------" << std::endl;

  char byte = 255;
  size_t size = 0;

  // check the last bit
  bool bit = byte >> 7;
  std::cout << "bit: " << bit << std::endl;

  EXPECT_TRUE(peterint::decode(byte, &size));
  std::cout << size << std::endl;

  byte = 100;
  EXPECT_FALSE(peterint::decode(byte, &size));
  std::cout << size << std::endl;
}
     
