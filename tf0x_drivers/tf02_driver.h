#pragma once
#ifndef TF0X_DRIVERS_TF02_DRIVER_H_
#define TF0X_DRIVERS_TF02_DRIVER_H_

#include <tf0x_drivers/config.h>
#include "export.h"
#include "tf0x_driver.h"
#include <memory>
#include "qt_serial_port.h"

namespace tf02_driver {
class API Driver : public tf0x_driver::Driver {
 public:
  bool Initialize();
  bool ReadDistance(double& distance) override;
  bool ReadMeasurement(double& distance, short& amplitude);
  bool SetPortName(const std::string& name);
 private:
  std::shared_ptr<tf0x_driver::QtSerialPort> serial_port_;
  std::string port_name_;
};
} // namespace tf02_driver

#endif // TF0X_DRIVERS_TF02_DRIVER_H_
