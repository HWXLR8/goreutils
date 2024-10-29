#include <client.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <thread>


Client::Client(std::string server_addr, int server_port) {
  connect(server_addr, server_port);
  // std::thread srv_read_th(&Client::readFromServer, this);
  // srv_read_th.detach();
}

Client::~Client() {
  close(sock_);
}

void Client::connect(std::string server_addr, int server_port) {
  if ((sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    throw std::runtime_error("failed to creat socket");
  }

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(server_port);

  // convert ipv4 and ipv6 addresses to binary
  if (inet_pton(AF_INET, server_addr.c_str(), &serv_addr.sin_addr) <= 0) {
    throw std::runtime_error("invalid server address");
  }

  if (::connect(sock_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    throw std::runtime_error("failed to connect to server");
  }
}

void Client::send(std::string msg) {
  // convert to network byte order
  uint32_t len = htonl(msg.length());
  // send length first
  ::send(sock_, &len, sizeof(len), 0);
  // now send the actual message
  ::send(sock_, msg.c_str(), msg.size(), 0);
}

int32_t Client::read(char* buffer, MSG_TYPE expected_msg_type) {
  uint8_t recvd_msg_type_8; // network representation of msg_type
  uint32_t len;
  int result;

  // read msg type
  result = ::read(sock_, &recvd_msg_type_8, 1);
  MSG_TYPE recvd_msg_type = static_cast<MSG_TYPE>(recvd_msg_type_8);
  // msg type as expected?
  if (recvd_msg_type != expected_msg_type) {
    // TERMINATE/DATA mismatches are the only type allowed
    if (recvd_msg_type != TERMINATE && expected_msg_type != DATA) {
      std::string err =
	std::string("Expected message type: ") + msgTypeToStr(expected_msg_type) +
	", Received message type: " + msgTypeToStr(recvd_msg_type);
      throw std::runtime_error(err);
    }
  }

  if (recvd_msg_type == TERMINATE) {
    return -1;
  }

  // read msg length first (4 bytes)
  result = ::read(sock_, &len, sizeof(len));
  if (result <= 0) {
    throw std::runtime_error("error reading message length from server");
  }

  // change byte order from network->host
  len = ntohl(len);

  // read actual message
  uint32_t total_read = 0;
  while (total_read < len) {
    result = ::read(sock_, buffer + total_read, len - total_read);
    if (result <= 0) {
      throw std::runtime_error("error reading message from server");
    }
    total_read += result;
  }

  return len;
}

void Client::ack() {
  uint8_t msg = 0xFF;
  ::send(sock_, &msg, 1, 0);
}

std::string Client::msgTypeToStr(const MSG_TYPE& msg_type) {
  switch (msg_type) {
  case FILE_NAME: return "FILE_NAME";
  case FILE_HASH: return "FILE_HASH";
  case DATA: return "DATA";
  case TERMINATE: return "TERMINATE";
  default: return "UNKNOWN";
  }
}
