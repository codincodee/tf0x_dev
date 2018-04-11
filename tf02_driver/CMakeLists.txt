configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

add_library(
  tf02_driver
    SHARED
      tf02_driver.c
      tf02_driver.h
      config.h.in
      export.h
)

set_target_properties(tf02_driver PROPERTIES
  PUBLIC_HEADER "tf02_driver.h;${CMAKE_CURRENT_BINARY_DIR}/config.h")

install(TARGETS tf02_driver
  # IMPORTANT: Add the tf02_driver library to the "export-set"
  EXPORT tf0x_devTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/tf02_driver"
    COMPONENT dev)
