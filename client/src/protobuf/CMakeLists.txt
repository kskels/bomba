
# generate protocol during cmake
EXECUTE_PROCESS(COMMAND ${PROTOC_TOOL} 
    --cpp_out=../client/src/protobuf protocol.proto 
    WORKING_DIRECTORY ${PROJECT_ROOT_DIR}/../server
  )

include_directories(
	${PROTOBUF_INC_DIR}
    ${PROJECT_ROOT_DIR}/src/protobuf
  )
add_library(bomba_protobuf
    protocol.pb.cc
  )

# regenerate protocol before every target build
# disabling due to protocol changes very slow
#add_custom_command(TARGET bomba_protobuf PRE_BUILD
#    COMMAND ${PROTOC_TOOL} 
#      --cpp_out=../client/src/protobuf protocol.proto 
#      WORKING_DIRECTORY ${PROJECT_ROOT_DIR}/../server
#  )

# add clean-up to the clean-gen
add_custom_target(clean_bomba_protobuf
    rm -rf ${PROJECT_ROOT_DIR}/src/protobuf/protocol*
  )
add_dependencies(clean-gen
  clean_bomba_protobuf
)

