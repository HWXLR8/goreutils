#pragma once

#include <stdint.h>

#include <string>
#include <optional>

class Client {
public:
  Client(std::string server_addr, int server_port);
  int32_t read(char* buffer);

private:
  int sock_;

  void connect(std::string server_addr, int server_port);
  void send(std::string msg);
};
