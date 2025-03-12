#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/token.h"
#include "../include/tokenizer.h"

using namespace std;

void writeTokensToFile(const vector<Token>& tokens, const string& filename) {
    ofstream out(filename);
    if (!out) {
        cerr << "无法打开输出文件" << endl;
        return;
    }
    
    for (const auto& token : tokens) {
        out << static_cast<int>(token.type) << "," << token.lexeme << "\n";
    }
}

int main() {
    ifstream in("./I&O/TestCases/sourceProgram1.txt");
    if (!in) {
        cerr << "无法找到输入文件" << endl;
        return -1;
    }
    
    stringstream buffer;
    buffer << in.rdbuf();
    string source = buffer.str();
    
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(source);
    
    writeTokensToFile(tokens, "./I&O/output/analyzedProgram1.txt");
    cout << "词法分析完成，结果已写入../I&O/output/analyzedProgram1.txt" << endl;
    
    return 0;
}