#ifndef TF02_TRAFFIC_DRIVER_H
#define TF02_TRAFFIC_DRIVER_H

#include "export.h"
#include "qt_serial_port.h"

namespace tf02_traffic_driver {
class API Driver {
 public:
  bool Initialize();
  bool ReadPrediction(bool& high);
  bool SetPortName(const std::string& name);
 private:
  std::shared_ptr<tf0x_driver::QtSerialPort> serial_port_;
  std::string port_name_;
};
} // namespace tf02_traffic_driver

#endif // TF02_TRAFFIC_DRIVER_H
