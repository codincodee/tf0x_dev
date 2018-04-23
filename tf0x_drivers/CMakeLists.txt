configure_file(
  config.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY
)

find_package(Qt5SerialPort)

add_library(
  tf0x_drivers
    SHARED
      tf02_driver.cpp
      tf02_driver.h
      tf02_traffic_driver.cpp
      tf02_traffic_driver.h
      tf0x_driver.cpp
      tf0x_driver.h
      abstract_serial_port.cpp
      abstract_serial_port.h
      qt_serial_port.cpp
      qt_serial_port.h
      config.h.in
      export.h
)

target_link_libraries(tf0x_drivers Qt5::SerialPort)

set(
  PUBLIC_HEADERS
    tf02_driver.h
    tf0x_driver.h
    abstract_serial_port.h
    qt_serial_port.h
    export.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

set_target_properties(tf0x_drivers PROPERTIES
  PUBLIC_HEADER
    "${PUBLIC_HEADERS}"
)

install(TARGETS tf0x_drivers
  # IMPORTANT: Add the tf0x_drivers library to the "export-set"
  EXPORT tf0x_devTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/tf0x_drivers"
    COMPONENT dev
)
