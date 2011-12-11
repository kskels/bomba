
include_directories(
    ${PROJECT_ROOT_DIR}/src/protobuf
    ${PROJECT_ROOT_DIR}/src/transport
    ${PROJECT_ROOT_DIR}/src/logging
  )

add_library(bomba_transport
    connection.cpp
  )

IF(GTEST_FOUND)
add_executable(transport_test EXCLUDE_FROM_ALL
    ${PROJECT_ROOT_DIR}/src/logging/log.cpp
    connection.cpp
    connection_test.cpp
    peterint_test.cpp
  )
target_link_libraries(transport_test
    bomba_protobuf
    ${GTEST_MAIN_LIB}
    ${GTEST_LIB}
    ${PROTOBUF_LIB}
    pthread
  )
add_dependencies(unittest
    transport_test
  )
ENDIF(GTEST_FOUND)

