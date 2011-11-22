
UNSET(IRRLICHT_INC_DIR CACHE)
FIND_PATH(IRRLICHT_INC_DIR irrlicht/irrlicht.h 
    HINTS /usr/include /usr/local/include $ENV{IRRLICHT_ROOT}/include 
    ${PROJECT_ROOT_DIR}/3pp/irrlicht/include
  )

UNSET(IRRLICHT_LIB CACHE)
FIND_LIBRARY(IRRLICHT_LIB Irrlicht
    HINTS /usr/lib /usr/local/lib $ENV{IRRLICHT_ROOT}/lib 
    ${PROJECT_ROOT_DIR}/3pp/irrlicht/lib
  )

IF(NOT IRRLICHT_INC_DIR OR NOT IRRLICHT_LIB)
  message(SEND_ERROR "Irrlicht could not be found")
ELSE(NOT IRRLICHT_INC_DIR OR NOT IRRLICHT_LIB)
  message(STATUS "Irrlicht found: 
     ${IRRLICHT_INC_DIR}/irrlicht/irrlicht.h 
     ${IRRLICHT_LIB}")
ENDIF(NOT IRRLICHT_INC_DIR OR NOT IRRLICHT_LIB)

