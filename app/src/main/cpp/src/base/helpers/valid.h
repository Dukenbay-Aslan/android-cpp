#pragma once

#include <string_view>

namespace valid {

bool ip(std::string_view string);
bool port(unsigned int number);
bool port(const std::string& numberStr);

} // namespace valid