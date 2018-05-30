#ifndef TF0X_DRIVER_UTILS_H_
#define TF0X_DRIVER_UTILS_H_

#include "export.h"
#include <string>
#include <vector>

namespace tf0x_driver {
class API Utils
{
 public:
  static std::string CommandLineInputToBuffer(const std::string& input);
  static std::string ToDecimalString(const char& i);
  static std::string ToHexString(const char& integer);
  static std::string ToBuffer(const int32_t& integer);
  static std::string ToInvertedBuffer(const int32_t& integer);
  static std::string ToBuffer(const std::vector<unsigned char>& integers);
};
}

#endif // TF0X_DRIVER_UTILS_H_
