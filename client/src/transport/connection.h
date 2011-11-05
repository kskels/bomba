// -*- c++ -*-

#ifndef BOMBA_CONNECTION_H
#define BOMBA_CONNECTION_H

#include <vector>

class NetMsg;

/**
 * All operations are nonblocking.
 */ 
class Connection {
public:
  enum State {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
  };
	
  void connect(const std::string &address, int port = 14242) {}
  void disconnect() {}
  void send(const NetMsg &) {}

  /**
   * Reads the next incoming message.
   * @return a pointer to the NetMsg received, or 0 if
   *         there's not enough data to construct a message.
   */	 
  NetMsg *receive(NetMsg &) const;

  State state() const;
};

#endif // !BOMBA_CONNECTION_H
