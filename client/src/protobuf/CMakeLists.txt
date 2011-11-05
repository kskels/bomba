
# generate protocol during cmake
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

# regenerate protocol every make-time
add_custom_target(gen_protobuf
    COMMAND ${PROTOC_TOOL} 
      --cpp_out=../client/src/protobuf protocol.proto 
      WORKING_DIRECTORY ${PROJECT_ROOT_DIR}/../server
  )
add_dependencies(bomba_protobuf
    gen_protobuf
  )

# add clean-up to the clean-gen
add_custom_target(clean_bomba_protobuf
    rm -rf ${PROJECT_ROOT_DIR}/src/protobuf/protocol*
  )
add_dependencies(clean-gen
  clean_bomba_protobuf
)

