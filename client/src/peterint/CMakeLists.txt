
include_directories(
    ${PROJECT_ROOT_DIR}/src/peterint
  )

IF(GTEST_FOUND)
add_executable(peterint_unittest EXCLUDE_FROM_ALL
    peterint_unittest.cpp
  )
target_link_libraries(peterint_unittest
    ${GTEST_MAIN_LIB}
    ${GTEST_LIB}
    pthread
  )
add_dependencies(unittest
    peterint_unittest
  )
ENDIF(GTEST_FOUND)

