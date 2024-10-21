#include <fstream>
#include <iostream>
#include <stdexcept>

#include <server.hpp>

enum LOG_STATE {
  GOOD,
  BAD,
};

void log(const std::string& text, LOG_STATE state) {
    const std::string RED = "\033[1;31m";
    const std::string GREEN = "\033[1;32m";
    const std::string RESET = "\033[0m";

    switch (state) {
    case GOOD:
      std::cout << GREEN << "* " << text << RESET << std::endl;
      break;
    case BAD:
      std::cout << RED << "* " << text << RESET << std::endl;
      break;
    }
}

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

    // send metadata

    while (in) {
      // try to read next chunk
      in.read(buffer, buffer_size);

      // get the number of bytes actually read
      size_t count = in.gcount();

      // check if we have reached EOF
      if (in.eof()) {
	log("Transfer complete", GOOD);
	// send whatever is left in the buffer
	server.send(buffer, count, 0x02);
	break;
      } else if (in.fail()) {
	throw std::runtime_error ("error reading from file");
	break;
      }

      server.send(buffer, count, 0x02);
    }

    // send done msg
    server.send(0, 0, 0x03);
    server.waitForAck();

    delete[] buffer;
  } catch (const std::exception& e) {
    log(e.what(), BAD);
  }
}
