#include "utils.h"
#include <sstream>

namespace tf0x_driver {
std::string Utils::CommandLineInputToBuffer(const std::string& input) {
  std::stringstream ss(input);
  std::string result;
  while (!ss.eof()) {
    std::string chs;
    ss >> chs;
    result += std::stoi(chs, 0, 16);
  }
  return result;
}
}
