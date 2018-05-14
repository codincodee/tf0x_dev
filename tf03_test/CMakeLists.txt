set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)
find_package(Qt5Charts)

add_executable(
  tf03_test
    main.cpp
    main_window.h
    main_window.cpp
    main_window_settings_page.cpp
    main_window_timer_event.cpp
    main_window_readings_page.cpp
    main_window_debug_page.cpp
    main_window.ui
)

target_link_libraries(
  tf03_test
    tf0x_common
    tf0x_drivers
    Qt5::Widgets
    Qt5::Charts
)

install(TARGETS tf03_test
  # IMPORTANT: Add the tf03_test executable to the "export-set"
  EXPORT tf02_testTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
