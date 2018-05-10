#ifndef TF03_DRIVER_H
#define TF03_DRIVER_H

#include "tf0x_driver.h"
#include "export.h"
#include <memory>
#include "abstract_serial_port.h"
#include <vector>

namespace tf03_driver {
struct Measurement {
  std::vector<unsigned short> dists; // cm
  unsigned short amb;
  short temp; // Celsius
  inline unsigned short DistanceMax() const {
    return 18000;
  }
};

class Driver: public tf0x_driver::Driver
{
 public:
  bool Initialize();
  void SetSerialPort(std::shared_ptr<tf0x_driver::AbstractSerialPort> port);
  bool ReadDistance(double &distance) override;
  bool ReadMeasurement(Measurement& measurement);
 private:
  std::shared_ptr<tf0x_driver::AbstractSerialPort> serial_port_;
};
} // namespace tf03_driver

#endif // TF03_DRIVER_H
