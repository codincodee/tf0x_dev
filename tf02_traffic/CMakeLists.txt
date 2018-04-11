add_executable(tf02_traffic main.cpp)

target_link_libraries(tf02_traffic tf0x_common)

install(TARGETS tf02_traffic
  # IMPORTANT: Add the tf02_traffic executable to the "export-set"
  EXPORT tf02_trafficTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
