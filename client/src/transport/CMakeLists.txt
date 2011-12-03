
include_directories(
    ${PROJECT_ROOT_DIR}/src/protobuf
    ${PROJECT_ROOT_DIR}/src/transport
    ${PROJECT_ROOT_DIR}/src/peterint
  )

add_library(bomba_transport
    connection.cpp
  )

IF(GTEST_FOUND)
add_executable(connection_test EXCLUDE_FROM_ALL
    connection.cpp
    connection_test.cpp
  )
target_link_libraries(connection_test
    bomba_protobuf
    ${GTEST_MAIN_LIB}
    ${GTEST_LIB}
    ${PROTOBUF_LIB}
    pthread
  )
add_dependencies(unittest
    connection_test
  )
ENDIF(GTEST_FOUND)

