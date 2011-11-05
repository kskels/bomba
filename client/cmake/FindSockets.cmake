
UNSET(SOCKETS_INC_DIR CACHE)
FIND_PATH(SOCKETS_INC_DIR Sockets/Socket.h 
    HINTS /usr/include /usr/local/include $ENV{SOCKETS_ROOT}/include
  )

UNSET(SOCKETS_LIB CACHE)
FIND_LIBRARY(SOCKETS_LIB Sockets
    HINTS /usr/lib /usr/local/lib $ENV{SOCKETS_ROOT}/lib
  )

IF(NOT SOCKETS_INC_DIR OR NOT SOCKETS_LIB)
  message(SEND_ERROR "Sockets could not be found")
ELSE(NOT SOCKETS_INC_DIR OR NOT SOCKETS_LIB)
  message(STATUS "Sockets found: 
     ${SOCKETS_INC_DIR}/Sockets/socket.h 
     ${SOCKETS_LIB}")
ENDIF(NOT SOCKETS_INC_DIR OR NOT SOCKETS_LIB)

