
#include <connection.h>
#include <peterint.hpp>
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
#include <errno.h>


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

  std::string size = peterint::encode(ss.str().size());
  if (::write(_sockfd, size.c_str(), size.size()) < 0) 
    throw "Failed to write size into the socket";

  if (::write(_sockfd, ss.str().c_str(), ss.str().size()) < 0)
    throw "Failed to write data into the socket"; 
}

NetMessage* Connection::receive() const {
  char byte;
  int n = ::recv(_sockfd, &byte, 1, MSG_DONTWAIT);
  if (n == 0 || errno == EAGAIN || errno == EWOULDBLOCK) 
    return 0;
  else if (n < 0)
    throw "Failed to recv from the socket";

  size_t size = 0;
  while(peterint::decode(byte, &size)) {
    if(::recv(_sockfd, &byte, 1, 0) < 0)
      throw "Failed to recv from the socket";
  }
 
  char buffer[size];
  bzero(buffer, size);
  n = ::recv(_sockfd, buffer, size, 0);
  if (n < 0)
    throw "Failed to recv from the socket"; 
  else if (n != size)
    throw "Failed to recv complete message";

  std::stringstream ss;
  for (int i(0); i != size; ++i)
    ss << buffer[i];

  NetMessage message; 
  message.ParseFromIstream(&ss);
  return new NetMessage(message); // a leak !!
}

Connection::State Connection::state() const {
  return _state;
}

