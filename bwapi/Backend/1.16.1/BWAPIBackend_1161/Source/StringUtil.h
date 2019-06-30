#pragma once

#include <functional>
#include <algorithm>
#include <cctype>

namespace Util {

inline std::string to_lower_copy(const std::string& s) {
  std::string result(s);
  std::transform(result.begin(), result.end(), result.begin(),
    [](std::string::const_reference c) { return static_cast<char>(std::tolower(c)); }
  );
  return result;
}

inline std::string to_upper_copy(const std::string& s) {
  std::string result(s);
  std::transform(result.begin(), result.end(), result.begin(),
    [](std::string::const_reference c) { return static_cast<char>(std::toupper(c)); }
  );
  return result;
}

// Trims the left side of str, using predicate to decide trimming (while true)
// cppcheck-suppress passedByValue
inline void trim_left(std::string& str, std::function<bool(char)> pred) {
  auto it = str.begin();
  auto it_end = str.end();
  // Skip first items while predicate gives true
  while (it != it_end && pred(*it)) {
    ++it;
  }
  // Drop from the beginning to here
  str.erase(str.begin(), it);
}

// Trims the right side of str, using pred to decide trimming (while true)
// cppcheck-suppress passedByValue
inline void trim_right(std::string& str, std::function<bool(char)> pred) {
  auto it = str.rbegin();
  auto it_end = str.rend();
  // Skip last items while predicate gives true
  while (it != it_end && pred(*it)) {
    ++it; // reverse iterator goes back
  }
  // Drop from here till the end
  str.erase(it.base(), str.end());
}

// Trims left and right side of str, using predicate to check if the character
// stays (false) or will be trimmed (true)
// cppcheck-suppress passedByValue
inline void trim(std::string& str, std::function<bool(char)> pred) {
  trim_left(str, pred);
  trim_right(str, pred);
}

//
// Predicate for trim
//

inline bool is_whitespace_or_newline(char c) {
  return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

} // Util
