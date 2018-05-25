#ifndef TF0X_DRIVER_UTILS_H_
#define TF0X_DRIVER_UTILS_H_

#include "export.h"
#include <string>

namespace tf0x_driver {
class API Utils
{
 public:
  static std::string CommandLineInputToBuffer(const std::string& input);
  static std::string ToDecimalString(const char& i);
};
}

#endif // TF0X_DRIVER_UTILS_H_
