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

std::string Utils::ToDecimalString(const char &integer) {
  std::stringstream ss;
  ss << (short)integer;
  return ss.str();
}

std::string Utils::ToHexString(const char &integer) {
  std::stringstream ss;
  ss << std::hex << (short)integer;
  return ss.str();
}

std::string Utils::ToBuffer(const int32_t &integer) {
  std::string buffer(sizeof(int32_t), 0);
  memcpy(&buffer[0], &integer, sizeof(int32_t));
  return buffer;
}

std::string Utils::ToInvertedBuffer(const int32_t &integer) {
  auto buffer = ToBuffer(integer);
  std::string inverted;
  inverted.reserve(buffer.size());
  for (auto i = buffer.rbegin(); i != buffer.rend(); ++i) {
    inverted.push_back(*i);
  }
}

std::string Utils::ToBuffer(const std::vector<unsigned char> &integers) {
  if (integers.empty()) {
    return "";
  }
  std::string buffer(integers.size(), 0);
  memcpy(&buffer[0], &integers[0], integers.size());
  return buffer;
}
}
