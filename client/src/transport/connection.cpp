
#include <connection.h>
#include <log.hpp>
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
  Log(INFO) << "Connecting to " << address << ":" << port << "..";

  struct sockaddr_in serv_addr;
  _sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (_sockfd < 0) {
    Log(INFO) << "Failed to open the socket";
    _state = DISCONNECTED;
    return;
  } 
  Log(DEBUG) << "Socket opened with value " << _sockfd;
  
  struct hostent *server = gethostbyname(address.c_str());
  if (!server) {
    Log(INFO) << "Failed to get host by name";
    _state = DISCONNECTED;
    return;
  } 
  Log(DEBUG) << "Host by name gotten";
 
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(port);
  if (::connect(_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    Log(INFO) << "Failed to connect";
    _state = DISCONNECTED;
    return;
  }
  Log(INFO) << "Connection established";

  _state = CONNECTED;  
}

void Connection::disconnect() {
  Log(INFO) << "Disconnecting..";
  ::shutdown(_sockfd, SHUT_RDWR);
  _state = DISCONNECTED;
}

void Connection::send(const NetMessage &message) {
  std::stringstream ss;
  message.SerializeToOstream(&ss);

  std::string size = peterint::encode(ss.str().size());
  if (::write(_sockfd, size.c_str(), size.size()) < 0) {
    Log(INFO) << "Failed to write size into the socket";
    _state = DISCONNECTED;
    return;
  }
  Log(DEBUG) << "Sent size in " << size.size() << " bytes";

  if (::write(_sockfd, ss.str().c_str(), ss.str().size()) < 0) {
    Log(INFO) << "Failed to write data into the socket";
    _state = DISCONNECTED;
    return;
  }
  Log(DEBUG) << "Sent data in " << ss.str().size() << " bytes";
}

NetMessage* Connection::receive(NetMessage& message) {
  char byte;
  int n = ::recv(_sockfd, &byte, 1, MSG_DONTWAIT);
  if (n == 0 || errno == EAGAIN || errno == EWOULDBLOCK) {
    return 0;
  } else if (n < 0) {
    Log(INFO) << "Failed to recv from the socket";
    _state = DISCONNECTED;
    return 0;
  }

  size_t size = 0;
  while(peterint::decode(byte, &size)) {
    if(::recv(_sockfd, &byte, 1, 0) < 0) {
      Log(INFO) << "Failed to recv from the socket";
      _state = DISCONNECTED;
      return 0;
    }
  }
 
  char buffer[size];
  bzero(buffer, size);
  n = ::recv(_sockfd, buffer, size, 0);
  if (n < 0) {
    Log(INFO) << "Failed to recv from the socket";
    _state = DISCONNECTED;
    return 0;
  } else if (n != size) {
    Log(INFO) << "Failed to recv complete message";
    _state = DISCONNECTED;
    return 0;
  }

  std::stringstream ss;
  for (int i(0); i != size; ++i)
    ss << buffer[i];
  message.ParseFromIstream(&ss);
  return &message;
}

Connection::State Connection::state() const {
  return _state;
}

