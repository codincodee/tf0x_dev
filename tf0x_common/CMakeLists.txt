configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Charts)

if("${TRAFFIC_COUNT_CORE_VERSION}" STREQUAL "")
  set(TRAFFIC_COUNT_CORE_VERSION "v1")
endif()

add_definitions(-DTRAFFIC_COUNT_CORE_${TRAFFIC_COUNT_CORE_VERSION})

add_library(
  tf0x_common
    SHARED
      tf0x_common.cpp
      tf0x_common.h
      distance_over_time_chart.cpp
      distance_over_time_chart.h
      config.h.in
      export.h
      tf02_traffic_count.cpp
      tf02_traffic_count.h
      tf02_traffic_count_core.cpp
      tf02_traffic_count_core.h
      distance_over_time_and_switch_value_chart.cpp
      distance_over_time_and_switch_value_chart.h
      distance_over_time_multi_series_chart.cpp
      distance_over_time_multi_series_chart.h
      qt_helpers.cpp
      qt_helpers.h
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
