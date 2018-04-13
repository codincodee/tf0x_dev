set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)
find_package(Qt5Charts)
find_package(Qt5Multimedia)
find_package(Qt5MultimediaWidgets)

add_executable(
  tf02_traffic
    main.cpp
    main_window.h
    main_window.cpp
    main_window.ui
    camera_window.h
    camera_window.cpp
    camera_window.ui
)

target_link_libraries(
  tf02_traffic
    tf0x_common
    Qt5::Widgets
    Qt5::Charts
    Qt5::Multimedia
    Qt5::MultimediaWidgets
)

install(TARGETS tf02_traffic
  # IMPORTANT: Add the tf02_traffic executable to the "export-set"
  EXPORT tf02_trafficTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
