
include_directories(
    ${PROJECT_ROOT_DIR}/src/peterint
  )

add_executable(peterint_unittest
    peterint_unittest.cpp
  )

target_link_libraries(peterint_unittest
    ${GTEST_MAIN_LIB}
    ${GTEST_LIB}
    pthread
  )

