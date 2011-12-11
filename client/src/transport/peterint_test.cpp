
#include <peterint.hpp>
#include <gtest/gtest.h>


class PeterintTest : public ::testing::Test {
protected:
  size_t size(const std::string& data) {
    EXPECT_TRUE(data.size());
    size_t size = 0;
    for (size_t i(0); i != data.size(); ++i) {
      bool more = peterint::decode(data[i], &size);
      if (i == (data.size()-1))
        EXPECT_FALSE(more);
      else
        EXPECT_TRUE(more);
    }
    return size;
  }
};

TEST_F(PeterintTest, TestEncodeDecode) 
{
    size_t values[] = {0, 1, 127, 128, 129, 12345, 1234567};
    for (size_t i(0); i != 7; ++i)
      EXPECT_EQ(values[i], size(peterint::encode(values[i])));
}

