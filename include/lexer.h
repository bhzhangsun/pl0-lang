#if !defined(__PL0_LEXER_H__)
#define __PL0_LEXER_H__

#include <iostream>
#include <fstream>
#include <list>
#include "token.h"

class Lexer
{
private:
    std::istream raw_stream;
    std::list<Token> token_stream;

public:
    Lexer(const string &filename)
    {
        std::ifstream fin;
        fin.open(filename, ios::in);
        raw_stream = fin;
    }
    Lexer(istream &in) : raw_stream(in)
    {
        tokenization();
    }

    ~Lexer()
    {
        if (raw_stream)
        {
            close(raw_stream);
        }
    }

    void tokenization();
}
#endif // __PL0_LEXER_H__
