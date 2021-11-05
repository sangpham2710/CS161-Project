#include "cmanip_utils.h"

#include <sstream>
#include <string>

std::string intToHexString(const int &number) {
  std::stringstream stream;
  stream << std::hex << number;
  return stream.str();
}
