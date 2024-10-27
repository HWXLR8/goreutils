#include <server.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>

Server::Server(unsigned int port, unsigned int max_connections) {
  // create socket
  sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ == -1) {
    throw std::runtime_error("failed to create socket");
  }

  // reuse ports to avoid "already in use" errors
  int optval = 1;
  setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval));

  // specify which port
  sockaddr_.sin_family = AF_INET;
  sockaddr_.sin_addr.s_addr = INADDR_ANY;
  sockaddr_.sin_port = htons(port);

  // bind to port
  if (bind(sock_, (struct sockaddr*)&sockaddr_, sizeof(sockaddr_)) < 0) {
    throw std::runtime_error("failed to bind to port");
  }

  // begin listening
  if (listen(sock_, max_connections) < 0) {
    throw std::runtime_error("failed to listen on socket");
  }
}

Server::~Server() {
  close(client_);
  close(sock_);
}

void Server::waitForClientConnection() {
  std::cout << "waiting for client connection..." << std::endl;
  auto addrlen = sizeof(sockaddr_);
  client_ = accept(sock_, (struct sockaddr*)&sockaddr_, (socklen_t*)&addrlen);
  if (client_ < 0) {
    throw std::runtime_error("failed to connect to client");
  }
  std::string client_ip_ = std::string(inet_ntoa(sockaddr_.sin_addr));
  std::cout << "connected to " << client_ip_ << std::endl;
}

void Server::read() {
  uint32_t len;
  int result;

  // read msg length first (4 bytes)
  result = ::read(client_, &len, sizeof(len));
  if (!result) {
    throw std::runtime_error("failed to read from client, is the socket alive?");
  }
  // change byte order from network->host
  len = ntohl(len);
  // read actual message
  char msg[len];
  result = ::read(client_, msg, len);
  if (!result) {
    throw std::runtime_error("failed to read from client, is the socket alive?");
  }
}

void Server::send(const char* buffer, size_t num_bytes, MSG_TYPE msg_type) {
  // send message type
  uint8_t msg_type_8 = static_cast<uint8_t>(msg_type);
  ::send(client_, &msg_type_8, 1, 0);

  // convert to network byte order
  uint32_t len = htonl(num_bytes);

  // send length first
  ::send(client_, &len, sizeof(len), 0);

  // now send the actual message
  ::send(client_, buffer, num_bytes, 0);
}

void Server::run() {
  waitForClientConnection();
}

void Server::waitForAck() {
  uint8_t msg;
  ::recv(client_, &msg, 1, 0);
  if (msg != 0xFF) {
    throw std::runtime_error("Incorrect ACK received");
  }
  return;
}
