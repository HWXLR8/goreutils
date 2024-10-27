#pragma once

#include <netinet/in.h>

#include <vector>
#include <string>

enum MSG_TYPE {
  FILE_NAME = 1,
  FILE_HASH = 2,
  DATA = 3,
  TERMINATE = 4,
};

class Server {
public:
  Server(unsigned int port, unsigned int max_connections);
  ~Server();
  void run();
  void waitForClientConnection();
  void read();
  void send(const char* buffer, size_t len, MSG_TYPE msg_type);
  void waitForAck();

private:
  int sock_;
  int client_;
  sockaddr_in sockaddr_;
  std::string msg_;
};
