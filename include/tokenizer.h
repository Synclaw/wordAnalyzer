#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>
#include "token.h"

class Tokenizer {
public:
    std::vector<Token> tokenize(const std::string& source);
    
private:
    void handleComment(size_t& pos, const std::string& source);
    bool isKeyword(const std::string& lexeme);
    bool isDelimiter(char c);
    bool isOperatorChar(char c);
    Token handleIdentifier(size_t& pos, const std::string& source);
    Token handleNumber(size_t& pos, const std::string& source);
    Token handleOperator(size_t& pos, const std::string& source);
    Token handleDelimiter(size_t& pos, const std::string& source);
    Token handleError(size_t& pos, const std::string& source);
};

#endif // TOKENIZER_H