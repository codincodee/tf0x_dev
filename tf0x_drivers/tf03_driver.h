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

enum class OutputFormat {
  ascii = 0x00,
  binary = 0x01,
  pix = 0x02,
  primitive = 0x03
};

class API Driver: public tf0x_driver::Driver
{
 public:
  bool Initialize();
  void SetSerialPort(std::shared_ptr<tf0x_driver::AbstractSerialPort> port);
  bool ReadDistance(double &distance) override;
  bool ReadMeasurement(Measurement& measurement);
  bool ReadMeasurement(Measurement &measurement, std::string& buffer);

  // Operations
  std::string GetVersion();
  bool ResetDevice();
  bool SetFrequency(const int& frequency);
  bool MeasureOnce(Measurement& Measurement);
  bool SetOutputFormat(const OutputFormat& format);
  bool SetBaudRate(const int& baud_rate);
  bool Enable(const bool& enable);
  bool EnableCheckSum(const bool& enable);
  bool RestoreFactory();
  bool SaveSettings();
 private:
  std::shared_ptr<tf0x_driver::AbstractSerialPort> serial_port_;
  static std::string Head();
  static std::string AppendCheckSum(const std::string& buffer);
};
} // namespace tf03_driver

#endif // TF03_DRIVER_H
