#include <ctype.h>
#include "lexer.h"

bool issymbol(char _c)
{
}

void Lexer::tokenization()
{
    size_t line_num = 0;
    while (!raw_stream.eof())
    {
        char pre_c = raw_stream.peek();
        if (pre_c == ' ' || pre_c == '\t')
        {
            raw_stream.get();
        }
        else if (pre_c == '\n')
        {
            raw_stream.get();
            line_num++;
        }
        else if (isdigit(pre_c))
        {
        }
        else if (isalpha(pre_c))
        {
        }
        else if (true /*symbol*/)
        {
        }
        else
        {
        }
    }
}