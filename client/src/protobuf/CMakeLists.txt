
EXECUTE_PROCESS(COMMAND ${PROTOC_TOOL} 
    --cpp_out=../client/src/protobuf protocol.proto 
    WORKING_DIRECTORY ${PROJECT_ROOT_DIR}/../server)

include_directories(
    ${PROJECT_ROOT_DIR}/src/protobuf
  )

add_library(bomba_protobuf
    protocol.pb.cc
  )

