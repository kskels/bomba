
UNSET(PROROBUB_INC_DIR CACHE)
FIND_PATH(PROTOBUF_INC_DIR google/protobuf/message.h 
    HINTS /usr/include /usr/local/include $ENV{PROTOBUF_ROOT}/include
)

UNSET(PROTOBUF_LIB CACHE)
FIND_LIBRARY(PROTOBUF_LIB protobuf
	HINTS /usr/lib /usr/local/lib $ENV{PROTOBUF_ROOT}/lib
)

IF(NOT PROTOBUF_INC_DIR OR NOT PROTOBUF_LIB)
    message(SEND_ERROR "Protobuf could not be found")
ELSE(NOT PROTOBUF_INC_DIR OR NOT PROTOBUF_LIB)
    message(STATUS "Protobuf found: 
        ${PROTOBUF_INC_DIR}/google/protobuf/message.h 
        ${PROTOBUF_LIB}")
ENDIF(NOT PROTOBUF_INC_DIR OR NOT PROTOBUF_LIB)

