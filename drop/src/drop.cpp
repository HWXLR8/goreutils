#include <fstream>
#include <iostream>
#include <stdexcept>

#include <server.hpp>

int main(int argc, char** argv) {
  try {
    if (argc == 1) {
      throw std::runtime_error("No file specified, exiting.");
    }

    size_t buffer_size = 1 << 20; // 1M
    char* buffer = new char[buffer_size];

    std::ifstream in(argv[1], std::ios::binary);

    unsigned int PORT = 6969;
    unsigned int MAX_CONN = 1;
    Server server(PORT, MAX_CONN);
    server.run();

    while (in) {
      // try to read next chunk
      in.read(buffer, buffer_size);
      // get the number of bytes actually read
      size_t count = in.gcount();
      // if we read nothing, abort
      if (!count) {
	break;
      }

      server.send(buffer, count);
    }
    delete[] buffer;
  } catch (const std::exception& e) {
    std::cerr << "\033[1;31mERROR: \033[0m" << e.what() << std::endl;
  }
}
