
#include <connection.h>
#include <peterint.hpp>
#include <protocol.pb.h>

#include <gtest/gtest.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>


static const unsigned _port = 14242; 
bool _accepted = false;
bool _received = false;
NetMessage _message;

std::string _got;

struct Server {
  static void* run(void *args) {
    try {
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0)
        throw std::string("Failed to open socket");

      int optval = 1;
      setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

      struct sockaddr_in serv_addr;
      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = INADDR_ANY;
      serv_addr.sin_port = htons(_port);
      if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        if (errno == EADDRINUSE) 
          throw std::string("Failed to bind, address already in use");
        else 
          throw std::string("Failed to bind");
      }

      listen(sockfd, 5);

      _ready = true;
      struct sockaddr_in cli_addr;
      socklen_t clilen; 
      int newsockfd;
      clilen = sizeof(cli_addr);
      newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
      std::cout << "newsockfd: " << newsockfd << std::endl;
      if (newsockfd < 0)
        throw std::string("Failed to accept"); 
      _accepted = true;

      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 1000;

      fd_set master, read_fds;
      FD_ZERO(&master);
      FD_ZERO(&read_fds);

      FD_SET(sockfd, &master);
      int fdmax = sockfd;
      std::cout << "sockfd: " << fdmax << std::endl;

      FD_SET(newsockfd, &master);
      if (newsockfd > fdmax)
        fdmax = newsockfd;

      while(_run) {
        read_fds = master;
        if (select(fdmax+1, &read_fds, NULL, NULL, &tv) == -1)
            throw std::string("Failed to select");

        for (int i(0); i != fdmax+1; ++i) {
        if (FD_ISSET(i, &read_fds) && i != sockfd) {
          std::cout << "got something: " << i << std::endl;
          char buffer[256];
          bzero(buffer, 256);
          int n = read(newsockfd, buffer, 2);

          size_t size = 0;
          bool more = true;
          char byte = '\0';
          
          while(more) {
            more = peterint::decode(byte, &size);
          }

          int16_t wait = atoi(buffer);
          std::cout << "Got size: " << wait << std::endl;

          bzero(buffer, 256);
          n = read(newsockfd, buffer, wait);
          std::cout << "got byets: " << n << std::endl;
          std::stringstream ss;
          for (int j(0); j != n; ++j) {
            std::cout << "." << std::endl;
            ss << buffer[j];
          }
          _got = ss.str();
          _message.ParseFromIstream(&ss);
          _received = true;
        }
        }
      }    

      close(newsockfd);
      close(sockfd);
    } catch (const std::string& error) {
        std::cout << "Error: " << error << std::endl;
        throw error;
    }
    pthread_exit(0);
  }
  void start() {
      _run = true;
      _ready = false;
      pthread_create(&_thread, 0, run, 0);
      while (!_ready);
  };
  void stop() {
    _run = false;
    pthread_join(_thread, 0);
  };
  static bool _run;
  static bool _ready;
  pthread_t _thread;
};
bool Server::_run, Server::_ready;

class ConnectionTest : public ::testing::Test {
protected:
  ConnectionTest() {
    _server.start();
  }
  virtual ~ConnectionTest() {
    _server.stop();
  }
private:
  Server _server;
};

TEST_F(ConnectionTest, TestConnection) {
  Connection connection;
  EXPECT_EQ(connection.state(), Connection::DISCONNECTED);

  _accepted = false;
  EXPECT_NO_THROW(connection.connect("localhost", _port));
  EXPECT_EQ(connection.state(), Connection::CONNECTED);
  while(!_accepted);

  EXPECT_NO_THROW(connection.disconnect());
  EXPECT_EQ(connection.state(), Connection::DISCONNECTED);
}

TEST_F(ConnectionTest, TestSend) {

  std::cout << sizeof(int16_t) << std::endl;

  Connection connection;
  EXPECT_NO_THROW(connection.connect("localhost", _port));
  EXPECT_EQ(connection.state(), Connection::CONNECTED);

  PositionUpdate position;
  position.set_entity(11);
  position.set_x(22);
  position.set_y(33); 

  NetMessage message;
  message.set_type(NetMessage::POSITION_UPDATE);
  *(message.mutable_player_position()) = position;

  EXPECT_TRUE(message.has_player_position());

  std::stringstream ss;
  message.SerializeToOstream(&ss);
  std::string sent = ss.str();
    
  _received = false;
  EXPECT_NO_THROW(connection.send(message));  

  while(!_received);

  EXPECT_EQ(sent, _got);

  EXPECT_EQ(_message.type(), NetMessage::POSITION_UPDATE);
  EXPECT_TRUE(_message.has_player_position());
  EXPECT_FALSE(_message.has_map());
  EXPECT_EQ(_message.player_position().entity(), 11);
}

