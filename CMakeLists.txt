cmake_minimum_required(VERSION 2.8)
project(tf0x_dev C CXX)

set(CMAKE_CXX_STANDARD 11)

set(tf0x_dev_MAJOR_VERSION 0)
set(tf0x_dev_MINOR_VERSION 1)
set(tf0x_dev_PATCH_VERSION 0)
set(tf0x_dev_VERSION
  ${tf0x_dev_MAJOR_VERSION}.${tf0x_dev_MINOR_VERSION}.${tf0x_dev_PATCH_VERSION})

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/tf0x_dev)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# set up include-directories
include_directories(
  "${PROJECT_SOURCE_DIR}"   # to find tf0x_drivers/tf0x_drivers.h
  "${PROJECT_BINARY_DIR}")  # to find tf0x_drivers/config.h

# Add sub-directories
add_subdirectory(tf0x_drivers)
add_subdirectory(tf0x_common)
add_subdirectory(tf02_traffic)
add_subdirectory(tf03_test)
add_subdirectory(tf03_traffic)
add_subdirectory(tf03_setup)

# The interesting stuff goes here
# ===============================

# Add all targets to the build-tree export set
export(
  TARGETS
    tf0x_drivers
    tf02_traffic
    tf0x_common
    tf03_test
    tf03_traffic
    tf03_setup
  FILE
    "${PROJECT_BINARY_DIR}/tf0x_devTargets.cmake"
)

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE tf0x_dev)

# Create the tf0x_devConfig.cmake and tf0x_devConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(tf0x_devConfig.cmake.in
  "${PROJECT_BINARY_DIR}/tf0x_devConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${tf0x_dev_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(tf0x_devConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/tf0x_devConfig.cmake" @ONLY)
# ... for both
configure_file(tf0x_devConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/tf0x_devConfigVersion.cmake" @ONLY)

# Install the tf0x_devConfig.cmake and tf0x_devConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/tf0x_devConfig.cmake"
  "${PROJECT_BINARY_DIR}/tf0x_devConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT tf0x_devTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
