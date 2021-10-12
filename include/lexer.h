#if !defined(__PL0_LEXER_H__)
#define __PL0_LEXER_H__

#include <istream>
#include <fstream>
#include <strstream>
#include <memory>
#include <list>

class Token;
class Lexer
{
private:
    std::unique_ptr<std::istream> raw_stream;
    std::list<Token> token_stream;

public:
    Lexer(const std::string &filename) : raw_stream(std::make_unique<>() std::ifstream(filename, std::ios::in))
    {
        tokenization();
    };
    // Lexer(std::ifstream &in) : raw_stream(std::ifstream(in))
    // {
    //     tokenization();
    // }
    // Lexer(std::strstream &in) : raw_stream(std::strstream(in))
    // {
    //     tokenization();
    // }

    ~Lexer() = default;

    void tokenization();

private:
    Token getWord();
    Token getNumber();
    Token getSymbol();
};

#endif // __PL0_LEXER_H__
