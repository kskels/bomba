// -*- c++ -*-

#ifndef BOMBA_CONNECTION_H
#define BOMBA_CONNECTION_H

#include <vector>
#include <string>

class NetMessage;

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
	
  void connect(const std::string &address, unsigned port = 14242) {};
  void disconnect() {};
  void send(const NetMessage &) {};

  /**
   * Reads the next incoming message.
   * @return a pointer to the NetMessage received, or 0 if
   *         there's not enough data to construct a message.
   */	 
  NetMessage *receive(NetMessage &) const {};

  State state() const {};
};

#endif // !BOMBA_CONNECTION_H
