
#include <connection.h>
#include <gtest/gtest.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


static const unsigned _port = 14242; 

struct Server {
  static void* run(void *args) {
    try {
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0)
        throw std::string("Failed to open socket");

      struct sockaddr_in serv_addr;
      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = INADDR_ANY;
      serv_addr.sin_port = htons(_port);
      if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        throw std::string("Failed to bind");

      listen(sockfd, 5);

      _ready = true;
      struct sockaddr_in cli_addr;
      socklen_t clilen; 
      int newsockfd;
      clilen = sizeof(cli_addr);
      newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
      if (newsockfd < 0)
        throw std::string("Failed to accept"); 
      std::cout << "Got connection.." << std::endl;

      while(_run);

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
    // probably good idea to fix this nicer
    // also, the clean-up might be needed
    pthread_kill(_thread, 0);
    //pthread_join(_thread, 0);
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

TEST_F(ConnectionTest, TestConnection)
{
  Connection connection;
  EXPECT_EQ(connection.state(), Connection::DISCONNECTED);

  EXPECT_NO_THROW(connection.connect("localhost", _port));
  EXPECT_EQ(connection.state(), Connection::CONNECTED);

  EXPECT_NO_THROW(connection.disconnect());
  EXPECT_EQ(connection.state(), Connection::DISCONNECTED);
}

