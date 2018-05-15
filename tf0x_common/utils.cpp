#include "utils.h"
#include <sstream>

namespace tf0x_common {
std::string Utils::ToHexString(const char &integer) {
  std::stringstream ss;
  ss << std::hex << (short)integer;
  return ss.str();
}
}
