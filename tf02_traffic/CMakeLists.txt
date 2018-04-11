add_executable(tf02_traffic main.c)

target_link_libraries(tf02_traffic tf0x_drivers)

install(TARGETS tf02_traffic
  # IMPORTANT: Add the tf02_traffic executable to the "export-set"
  EXPORT tf02_trafficTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
