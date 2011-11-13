
#include <connection.h>
#include <gtest/gtest.h>


static const unsigned _port = 27001; 

class Server {
public:
  Server(unsigned port = _port) {
    // init
  }
  ~Server() {
    // clean-up
  }
  void start() {
    // open a new thread
  };
  void stop() {
    // join the thread
  };
};

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

