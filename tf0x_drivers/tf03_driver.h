#ifndef TF03_DRIVER_H
#define TF03_DRIVER_H

#include "tf0x_driver.h"
#include "export.h"
#include <memory>
#include "abstract_serial_port.h"
#include <vector>
#include <QElapsedTimer>

namespace tf03_driver {
struct Measurement {
  uint16_t dist1;
  uint16_t dist2;
  uint16_t dist3;
  unsigned char apd;
  uint16_t volt;
  uint16_t temp; // Celsius
  inline uint16_t DistanceMax() const {
    return 18000;
  }
  long ts;
};

struct CartMeasurement {
  CartMeasurement() : id(0), pos(0) {}
  int id;
  unsigned short pos;
  Measurement measurement;
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
  QElapsedTimer timer_;
};
} // namespace tf03_driver

#endif // TF03_DRIVER_H
