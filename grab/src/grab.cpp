#include <fstream>
#include <iostream>

#include <client.hpp>

int main(int argc, char** argv) {
  size_t buffer_size = 1 << 20; // 1M
  char* buffer = new char[buffer_size];

  std::fstream out("out.png", std::ios::out | std::ios::binary);

  std::string ADDR = "127.0.0.1";
  int PORT = 6969;
  Client client(ADDR, PORT);

  size_t tx_total = 0;
  while (true) {
    uint32_t len;
    len = client.read(buffer);
    if (len == -1) {
      std::cout << "\nDownload complete" << std::endl;
      break;
    }
    tx_total += len;
    std::cout << "\rTotal bytes received " << tx_total << " B" << std::flush;
    out.write(buffer, len);
  }

  return 0;
}
