
include_directories(
    ${SOCKETS_INC_DIR}
  )

add_executable(server
    DisplaySocket.cpp
    server.cpp
  )

add_executable(client
    client.cpp
  )

target_link_libraries(server
    ${SOCKETS_LIB}
    pthread
    ssl
  )
target_link_libraries(client
    ${SOCKETS_LIB}
    pthread
    ssl
  )

