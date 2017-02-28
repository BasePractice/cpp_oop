#ifndef CPP_OOP_COMMON_TESTS_H
#define CPP_OOP_COMMON_TESTS_H

#include <string>

#if defined(_WIN32)
#define EOL "\n"
#else
#define EOL "\r\n"
#endif

const std::string
file_content(const char * const filename, bool *ok);

const std::vector<std::string>
file_content_lines(const char * const filename, bool *ok);

#endif //CPP_OOP_COMMON_TESTS_H
