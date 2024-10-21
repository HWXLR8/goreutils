#pragma once

#include <netinet/in.h>

#include <vector>
#include <string>

class Server {
public:
  Server(unsigned int port, unsigned int max_connections);
  ~Server();
  void run();
  void waitForClientConnection();
  void read();
  void send(char* buffer, size_t len, uint8_t msg_type);
  void sendMetadata(char* buffer, size_t len);

private:
  int sock_;
  int client_;
  sockaddr_in sockaddr_;
  std::string msg_;
};
