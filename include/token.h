#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType
{
    KEYWORD = 1,
    IDENTIFIER,
    INTEGER,
    FLOAT,
    OPERATOR,
    DELIMITER,
    ERROR
};

struct Token
{
    TokenType type;
    std::string lexeme;
};

#endif // TOKEN_H