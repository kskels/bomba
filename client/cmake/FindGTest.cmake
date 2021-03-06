
UNSET(GTEST_INCLUDE_DIR CACHE)
FIND_PATH(GTEST_INCLUDE_DIR gtest/gtest.h
    HINTS /usr/include/ /usr/local/include $ENV{GTEST_ROOT}/include
  )

UNSET(GTEST_LIB CACHE)
FIND_LIBRARY(GTEST_LIB gtest
    HINTS /usr/lib /usr/local/lib $ENV{GTEST_ROOT}/lib
  )

UNSET(GTEST_MAIN_LIB CACHE)
FIND_LIBRARY(GTEST_MAIN_LIB gtest_main
    HINTS /usr/lib /usr/local/lib $ENV{GTEST_ROOT}/lib
  )

UNSET(GTEST_FOUND CACHE)
IF(GTEST_INCLUDE_DIR AND GTEST_LIB AND GTEST_MAIN_LIB)
  MESSAGE(STATUS "Google Test found:
     ${GTEST_INCLUDE_DIR}/gtest/gtest.h
     ${GTEST_LIB}
     ${GTEST_MAIN_LIB}")
	SET(GTEST_FOUND true)
ELSE(GTEST_INCLUDE_DIR AND GTEST_LIB AND GTEST_MAIN_LIB)
  MESSAGE(STATUS "Google Test could not be found")
ENDIF(GTEST_INCLUDE_DIR AND GTEST_LIB AND GTEST_MAIN_LIB)

