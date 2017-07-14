// In the header file

#include <sstream>

class Salutation {
public:
  static std::string greet(const std::string& name);
};

// In the class implementation file
std::string Salutation::greet(const std::string& name) {
  std::ostringstream s;
  s << "Hello " << name << "!";
  return s.str();
}

// In the test file
#include <gtest/gtest.h>

TEST(SalutationTest, Static) {
  EXPECT_EQ(std::string("Hello World!"), Salutation::greet("World"));
}
