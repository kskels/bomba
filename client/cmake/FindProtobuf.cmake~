
UNSET(PROROBUB_INC_DIR CACHE)
FIND_PATH(PROTOBUF_INC_DIR google/protobuf/message.h 
    HINTS /usr/include /usr/local/include $ENV{PROTOBUF_ROOT}/include
    ${PROJECT_ROOT_DIR}/3pp/protobuf/include
  )


UNSET(PROTOBUF_LIB CACHE)
FIND_LIBRARY(PROTOBUF_LIB protobuf
    HINTS /usr/lib /usr/local/lib $ENV{PROTOBUF_ROOT}/lib
    ${PROJECT_ROOT_DIR}/3pp/protobuf/lib
  )

UNSET(PROTOC_TOOL CACHE)
FIND_PROGRAM(PROTOC_TOOL protoc
    HINTS /usr/bin /usr/local/bin $ENV{PROTOBUF_ROOT}/bin
    ${PROJECT_ROOT_DIR}/3pp/protobuf/bin
  )

MESSAGE(STATUS "!!!${PROTOBUF_TOOL}")

IF(NOT PROTOBUF_INC_DIR OR NOT PROTOBUF_LIB OR NOT PROTOC_TOOL)
  message(SEND_ERROR "Protobuf could not be found")
ELSE(NOT PROTOBUF_INC_DIR OR NOT PROTOBUF_LIB OR NOT PROTOC_TOOL)
  message(STATUS "Protobuf found: 
     ${PROTOBUF_INC_DIR}/google/protobuf/message.h 
     ${PROTOBUF_LIB}
     ${PROTOC_TOOL}")
ENDIF(NOT PROTOBUF_INC_DIR OR NOT PROTOBUF_LIB OR NOT PROTOC_TOOL)

