
include_directories(
    ${IRRLICHT_INC_DIR}
  )

add_executable(bomba
    main.cc   
  )

target_link_libraries(bomba
    ${IRRLICHT_LIB}
    bomba_protobuf
  )

add_subdirectory(protobuf)
add_subdirectory(transport)

