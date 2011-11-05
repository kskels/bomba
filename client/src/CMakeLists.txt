
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


if (APPLE)
  set(CMAKE_CXX_FLAGS "-m32")
  find_package(Cocoa REQUIRED)
  find_package(Carbon REQUIRED)
  find_package(IOKit REQUIRED)
  find_package(OpenGL REQUIRED)
  
  include_directories(
    ${Cocoa_INCLUDE_DIR}
	${IOKit_INCLUDE_DIR}
	${Carbon_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
    )
  
  target_link_libraries(bomba
    ${Cocoa_LIBRARIES}
	${IOKit_LIBRARIES}
	${Carbon_LIBRARIES}
	${OPENGL_LIBRARIES}
    )
endif (APPLE)


add_subdirectory(protobuf)
add_subdirectory(transport)

