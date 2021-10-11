#if !defined(__PL0_TOKEN_H__)
#define __PL0_TOKEN_H__

#include <string>

class Token
{
private:
    std::string value;
    // type
    size_t line;

public:
    Token(const std::string &str, size_t ln) : value(str), line(ln){};
    ~Token() = default;
};

#endif // __PL0_TOKEN_H__
