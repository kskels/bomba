
#include <connection.h>
#include <protocol.pb.h>

#include <iostream>
#include <sstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


void Connection::connect(const std::string &address, unsigned port) {
    _state = CONNECTING;

    struct sockaddr_in serv_addr;
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0) 
        throw std::string("Failed to open the socket");

    struct hostent *server = gethostbyname(address.c_str());
    if (!server) 
        throw std::string("Failet to get host by name");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    if (::connect(_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        throw std::string("Failed to connect");

    _state = CONNECTED;  
}

void Connection::disconnect() {
  ::shutdown(_sockfd, SHUT_RDWR);
  _state = DISCONNECTED;
}

void Connection::send(const NetMessage &message) {
  std::stringstream ss;
  message.SerializeToOstream(&ss);

  std::stringstream size;
  size << ss.str().size();

  int n = ::write(_sockfd, size.str().c_str(), sizeof(int16_t));
  if (n < 0)
    throw std::string("Failed to write size into the socket");

  n = ::write(_sockfd, ss.str().c_str(), ss.str().size());
  if (n < 0) 
    throw std::string("Failed to write data into the socket"); 
  std::cout << "Sent " << n << " bytes =]" << std::endl;
}

Connection::State Connection::state() const {
  return _state;
}
