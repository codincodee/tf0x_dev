#ifndef TF0X_DRIVERS_TF0X_DRIVER_H_
#define TF0X_DRIVERS_TF0X_DRIVER_H_

#include "export.h"

namespace tf0x_driver {
class API Driver
{
public:
  virtual bool ReadDistance(double& distance) = 0;
};
} // namespace tf0x_driver

#endif // TF0X_DRIVERS_TF0X_DRIVER_H_
