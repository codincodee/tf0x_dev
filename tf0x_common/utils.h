#ifndef TF0X_COMMON_UTILS_H_
#define TF0X_COMMON_UTILS_H_

#include "export.h"
#include <string>

namespace tf0x_common {
class API Utils
{
 public:
  static std::string ToHexString(const char& integer);
};
}

#endif // TF0X_COMMON_UTILS_H_
