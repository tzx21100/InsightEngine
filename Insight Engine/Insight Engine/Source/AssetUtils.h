#include <iostream>
#include <string>

#define ON_ERROR_EXIT(condition, msg) \
do { \
    if (condition) { \
        std::cerr << "Error in function: " << __func__ << " at line " << __LINE__ << std::endl; \
        perror(msg); \
    } \
} while (0);

static inline bool stringEndsIn(const std::string& str, const std::string& ends) {
    size_t str_len = str.length();
    size_t ends_len = ends.length();
    size_t pos = str.rfind(ends);
    return (pos != std::string::npos) && (pos + ends_len == str_len);
}