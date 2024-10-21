#pragma once

#include <string>

enum LOG_STATE {
  GOOD,
  BAD,
};

void log(const std::string& text, LOG_STATE state);
