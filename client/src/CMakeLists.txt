
include_directories(
    ${IRRLICHT_INC_DIR}
    ${PROJECT_ROOT_DIR}/src
    ${PROJECT_ROOT_DIR}/src/transport
    ${PROJECT_ROOT_DIR}/src/protobuf
    ${PROJECT_ROOT_DIR}/src/logging
  )

FILE(GLOB BOMBA_SOURCE_FILES "*.cpp")
add_executable(bomba
    ${PROJECT_ROOT_DIR}/src/logging/log.cpp
    ${BOMBA_SOURCE_FILES}
  )

target_link_libraries(bomba
    bomba_protobuf
    bomba_transport
    ${IRRLICHT_LIB}
    ${PROTOBUF_LIB}
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
add_subdirectory(peterint)

