configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Charts)

add_library(
  tf0x_common
    SHARED
      tf0x_common.cpp
      tf0x_common.h
      distance_over_time_chart.cpp
      distance_over_time_chart.h
      config.h.in
      export.h
)

target_link_libraries(
  tf0x_common
    tf0x_drivers
    Qt5::Charts
)

set(
  PUBLIC_HEADERS
    tf0x_common.h
    export.h
    distance_over_time_chart.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

set_target_properties(tf0x_common PROPERTIES
  PUBLIC_HEADER
    "${PUBLIC_HEADERS}"
)

install(TARGETS tf0x_common
  # IMPORTANT: Add the tf0x_common library to the "export-set"
  EXPORT tf0x_devTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/tf0x_common"
    COMPONENT dev)
