#pragma once

#include <stdint.h>

#include <string>
#include <optional>

enum MSG_TYPE {
  FILE_NAME = 1,
  FILE_HASH = 2,
  DATA = 3,
  TERMINATE = 4,
};

class Client {
public:
  Client(std::string server_addr, int server_port);
  ~Client();
  int32_t read(char* buffer, MSG_TYPE msg_type);
  void ack();

private:
  int sock_;

  void connect(std::string server_addr, int server_port);
  void send(std::string msg);
  std::string msgTypeToStr(const MSG_TYPE& msg_type);
};
