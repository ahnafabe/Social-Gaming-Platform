#include "data/session/helpers.h"

// Credit to Carl on StackOverflow for this function
// https://stackoverflow.com/a/12468109
std::string randomString(size_t length)
{
  auto randchar = []() -> char
  {
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[rand() % max_index];
  };
  std::string str(length, 0);
  std::generate_n(str.begin(), length, randchar);
  return str;
}