#include "tf03_driver.h"

namespace tf03_driver {
bool Driver::Initialize() {
  if (!serial_port_) {
    return false;
  }
  return true;
}

bool Driver::ReadDistance(double &dist0) {
  Measurement measurement;
  if (!ReadMeasurement(measurement)) {
    return false;
  }
  if (measurement.dists.empty()) {
    return false;
  }
  dist0 = measurement.dists[0] / 100.0;
  return true;
}

bool Driver::ReadMeasurement(Measurement& measurement, std::string& buffer) {
  if (!serial_port_) {
    return false;
  }
  // TODO: Implementations
  return true;
}

bool Driver::ReadMeasurement(Measurement &measurement) {
//  std::string buffer;
//  return ReadMeasurement(measurement, buffer);

  static float dist = 10.0f;
  static float sign = 1.0f;
  if (dist > 150.0f) {
    sign *= -1.0f;
  }
  if (dist < 1.0f) {
    sign *= -1.0f;
  }
  dist += sign * 1;
  measurement.dists.push_back(dist);
  return true;
}

void Driver::SetSerialPort(
    std::shared_ptr<tf0x_driver::AbstractSerialPort> port) {
  serial_port_ = port;
}
}
