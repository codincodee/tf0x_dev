#ifndef TF0X_DEV_TF0X_DRIVER_CART_DRIVER_H_
#define TF0X_DEV_TF0X_DRIVER_CART_DRIVER_H_

#include "abstract_serial_port.h"
#include <memory>
#include "export.h"

// Just for simulation
#include <QElapsedTimer>

namespace cart_driver {
struct Instruction {
 enum class Type {read_sensor, reach_end_point, reach_start_point};
 Type type;
};

class API Driver {
 public:
  bool Initialize();
  void SetSerialPort(std::shared_ptr<tf0x_driver::AbstractSerialPort> serial);
  bool ReadInstruction(Instruction& instruction);
  bool ReadInstruction(Instruction& instruction, int& repetition);
  bool StartMultiStopsTesting(const float& distance, const float& delta);
  float TotalDistance();
  float StopInterval();
 private:
  std::shared_ptr<tf0x_driver::AbstractSerialPort> serial_port_;

  // Just for simulation
  float distance_;
  float delta_;
  float current_;
  QElapsedTimer timer_;
};
} // namespace cart_driver

#endif // TF0X_DEV_TF0X_DRIVER_CART_DRIVER_H_
