set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)
# find_package(Qt5Charts)

add_executable(
  tf03_setup
    main.cpp
    driver_parsers.cpp
    driver.cpp
    driver.h
    parsed.cpp
    parsed.h
    main_window.cpp
    main_window.h
    main_window.ui
    static_unique_ptr_cast.h
)

target_link_libraries(
  tf03_setup
    tf0x_common
    tf0x_drivers
    Qt5::Widgets
)

install(TARGETS tf03_setup
  # IMPORTANT: Add the tf03_setup executable to the "export-set"
  EXPORT tf0x_devTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
