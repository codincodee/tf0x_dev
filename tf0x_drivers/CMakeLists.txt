configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

add_library(
  tf0x_drivers
    SHARED
      tf02_driver.cpp
      tf02_driver.h
      config.h.in
      export.h
)

set_target_properties(tf0x_drivers PROPERTIES
  PUBLIC_HEADER "tf02_driver.h;${CMAKE_CURRENT_BINARY_DIR}/config.h")

install(TARGETS tf0x_drivers
  # IMPORTANT: Add the tf0x_drivers library to the "export-set"
  EXPORT tf0x_devTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/tf0x_drivers"
    COMPONENT dev)
