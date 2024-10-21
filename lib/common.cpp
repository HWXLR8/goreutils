#include "common.hpp"

#include <iostream>

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
