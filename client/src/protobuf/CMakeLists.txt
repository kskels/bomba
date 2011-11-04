
EXECUTE_PROCESS(COMMAND ${PROTOC_TOOL} 
    --cpp_out=../client/src/protobuf protocol.proto 
    WORKING_DIRECTORY ${PROJECT_ROOT_DIR}/../server
  )

include_directories(
    ${PROJECT_ROOT_DIR}/src/protobuf
  )

add_library(bomba_protobuf
    protocol.pb.cc
  )

add_custom_target(gen_protobuf
  COMMAND ${PROTOC_TOOL} 
    --cpp_out=../client/src/protobuf protocol.proto 
    WORKING_DIRECTORY ${PROJECT_ROOT_DIR}/../server
)

add_dependencies(bomba_protobuf
  gen_protobuf
)

