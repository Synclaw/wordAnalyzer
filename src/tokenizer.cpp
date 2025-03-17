#include "../include/tokenizer.h"
#include <unordered_set>
#include <cctype>

namespace
{
    // 定义关键字集合
    const std::unordered_set<std::string> KEYWORDS = {"if", "else", "while", "for", "int", "float", "return", "void"};

    // 定义操作符集合
    const std::unordered_set<std::string> OPERATORS = {"+", "-", "*", "/", "=", "==", "!=", "<", "<=", ">", ">=", "&&", "||", "!", "&", "|"};

    // 定义分隔符集合
    const std::unordered_set<char> DELIMITERS = {';', ',', '(', ')', '{', '}', '[', ']', ':', '.'};
}

// 处理注释
void Tokenizer::handleComment(size_t &pos, const std::string &source)
{
    if (pos + 1 >= source.size())
        return;

    if (source[pos + 1] == '/')
    { // 行注释
        pos += 2;
        while (pos < source.size() && source[pos] != '\n')
            pos++;
    }
    else if (source[pos + 1] == '*')
    { // 块注释
        pos += 2;
        while (pos < source.size())
        {
            if (source[pos] == '*' && pos + 1 < source.size() && source[pos + 1] == '/')
            {
                pos += 2;
                return;
            }
            pos++;
        }
    }
}

// 判断是否为关键字
bool Tokenizer::isKeyword(const std::string &lexeme)
{
    return KEYWORDS.count(lexeme);
}

// 判断是否为分隔符
bool Tokenizer::isDelimiter(char c)
{
    return DELIMITERS.count(c);
}

// 判断是否为操作符
bool Tokenizer::isOperatorChar(char c)
{
    static const std::string op_chars = "+-*/=!<>&|";
    return op_chars.find(c) != std::string::npos;
}

// 处理标识符
Token Tokenizer::handleIdentifier(size_t &pos, const std::string &source)
{
    size_t start = pos;
    while (pos < source.size() && (isalnum(source[pos]) || source[pos] == '_'))
    {
        pos++;
    }
    std::string lexeme = source.substr(start, pos - start);
    return {isKeyword(lexeme) ? TokenType::KEYWORD : TokenType::IDENTIFIER, lexeme};
}

// 处理数字
Token Tokenizer::handleNumber(size_t &pos, const std::string &source)
{
    size_t start = pos;
    bool has_dot = false;
    int state = 0; // 0: init, 1: int, 2: dot, 3: fraction

    while (pos < source.size())
    {
        char c = source[pos];
        if (isdigit(c))
        {
            if (state == 0)
                state = 1;
            else if (state == 2)
                state = 3;
            pos++;
        }
        else if (c == '.' && !has_dot)
        {
            has_dot = true;
            state = 2;
            pos++;
        }
        else
        {
            break;
        }
    }

    std::string lexeme = source.substr(start, pos - start);

    // 有效性检查
    if (has_dot)
    {
        if (lexeme == "." || lexeme.back() == '.' ||
            (lexeme.find('.') == 0 && lexeme.size() == 1))
        {
            return {TokenType::ERROR, lexeme};
        }
        return {TokenType::FLOAT, lexeme};
    }
    return {TokenType::INTEGER, lexeme};
}

// 处理操作符
Token Tokenizer::handleOperator(size_t &pos, const std::string &source)
{
    size_t start = pos;
    pos++;
    if (start + 1 < source.size())
    {
        std::string possible_op = source.substr(start, 2);
        if (OPERATORS.count(possible_op))
        {
            pos++;
            return {TokenType::OPERATOR, possible_op};
        }
    }
    std::string op = source.substr(start, 1);
    return OPERATORS.count(op) ? Token{TokenType::OPERATOR, op} : Token{TokenType::ERROR, op};
}

// 处理分隔符
Token Tokenizer::handleDelimiter(size_t &pos, const std::string &source)
{
    return {TokenType::DELIMITER, std::string(1, source[pos++])};
}

Token Tokenizer::handleError(size_t &pos, const std::string &source)
{
    return {TokenType::ERROR, std::string(1, source[pos++])};
}

// 遍历文档，将文档中的字符转换为Token
std::vector<Token> Tokenizer::tokenize(const std::string &source)
{
    std::vector<Token> tokens;
    size_t pos = 0; // 初始化当前位置为0

    while (pos < source.size())
    {
        // 跳过空白
        while (pos < source.size() && isspace(source[pos]))
            pos++;
        if (pos >= source.size())
            break;

        char c = source[pos];

        if (pos + 1 < source.size() && source[pos] == '/' && (source[pos + 1] == '/' || source[pos + 1] == '*'))
        {
            handleComment(pos, source); // 处理注释
        }
        else if (isdigit(source[pos + 1]))
        {
            tokens.push_back(handleNumber(pos, source)); // 处理浮点数
        }
        else if (isDelimiter(c))
        {
            tokens.push_back(handleDelimiter(pos, source)); // 处理分隔符
        }
        else if (isalpha(c) || c == '_')
        {
            tokens.push_back(handleIdentifier(pos, source)); // 处理标识符
        }
        else if (isdigit(c))
        {
            tokens.push_back(handleNumber(pos, source)); // 处理数字
        }
        else if (isOperatorChar(c))
        {
            tokens.push_back(handleOperator(pos, source)); // 处理操作符
        }
        else if (isDelimiter(c))
        {
            tokens.push_back(handleDelimiter(pos, source)); // 处理分隔符
        }
        else
        {
            tokens.push_back(handleError(pos, source)); // 处理错误
        }
        break;
    }

    return tokens;
}