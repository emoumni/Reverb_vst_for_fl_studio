# ConfigureFile.cmake -- cmake -P script to run configure_file

if(NOT CONFIGURE_FILE_INPUT)
  message(FATAL_ERROR "need variable CONFIGURE_FILE_INPUT not found")
endif()
if(NOT CONFIGURE_FILE_OUTPUT)
  message(FATAL_ERROR "need variable CONFIGURE_FILE_OUTPUT not found")
endif()
configure_file("${CONFIGURE_FILE_INPUT}" "${CONFIGURE_FILE_OUTPUT}")
