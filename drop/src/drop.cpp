#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include <server.hpp>

#include "../../lib/common.hpp"


int main(int argc, char** argv) {
  try {
    if (argc == 1) {
      throw std::runtime_error("No file specified, exiting.");
    }

    // file info
    std::string file_path = argv[1];
    std::ifstream input_stream(file_path, std::ios::binary);
    std::string file_name = std::filesystem::path(file_path).filename().string();

    // allocate buffer
    size_t buffer_size = 1 << 20; // 1M
    char* buffer = new char[buffer_size];

    // initialize server
    unsigned int PORT = 6969;
    unsigned int MAX_CONN = 1;
    Server server(PORT, MAX_CONN);
    server.run();

    // send metadata
    server.send(file_name.c_str(), file_name.size(), FILE_NAME);

    while (input_stream) {
      // try to read next chunk
      input_stream.read(buffer, buffer_size);

      // get the number of bytes actually read
      size_t count = input_stream.gcount();

      // check if we have reached EOF
      if (input_stream.eof()) {
	log("Transfer complete", GOOD);
	// send whatever is left in the buffer
	server.send(buffer, count, DATA);
	break;
      } else if (input_stream.fail()) {
	throw std::runtime_error ("error reading from file");
	break;
      }

      server.send(buffer, count, DATA);
    }

    // send done msg
    server.send(0, 0, TERMINATE);
    server.waitForAck();

    delete[] buffer;
  } catch (const std::exception& e) {
    log(e.what(), BAD);
  }
}
