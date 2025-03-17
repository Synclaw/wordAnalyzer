#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <filesystem>
#include "../include/token.h"
#include "../include/tokenizer.h"

#define inputDir "./IO/TestCases/" // 在此填写源码目录
#define outputDir "./IO/output/"   // 在此填写输出目录

using namespace std;

vector<string> FileQueue()
{
    vector<string> file_list;
    try
    {
        for (const auto &entry : filesystem::directory_iterator(inputDir))
        {
            file_list.push_back(entry.path().filename().string());
        }
    }
    catch (const filesystem::filesystem_error &e)
    {
        throw runtime_error("目录访问失败: " + string(e.what()));
    }
    return file_list;
}

bool writeTokensToFile(const vector<Token> &tokens, const string &filename)
{
    ofstream out(filename);
    if (!out)
    {
        cout << "无法打开输出文件：" << filename << endl;
        return true;
    }
    for (const auto &token : tokens)
    {
        out << static_cast<int>(token.type) << "," << token.lexeme << "\n";
    }
    return false;
}

int main()
{
    try
    {
        vector<string> fileList = FileQueue();

        for (const auto &file : fileList)
        {
            string CurrentFileName = inputDir + file;
            string CurrentOutput = outputDir + file;
            cout << "当前文件: " << file << endl;
            ifstream in(CurrentFileName);
            if (!in)
            {
                cout << "无法找到输入文件: " << file << endl;
                continue;
            }
            else
            {
                stringstream buffer;
                buffer << in.rdbuf();
                Tokenizer tokenizer;
                auto tokens = tokenizer.tokenize(buffer.str());
                if (writeTokensToFile(tokens, CurrentOutput))
                {
                    cout << "写入文件失败" << endl;
                    break;
                }
                else
                {
                    cout << "词法分析完成，结果已写入: " << CurrentOutput << endl;
                }
            }
        }
        cout << "队列已完成，请到输出文件夹查看" << endl;
    }
    catch (const exception &e)
    {
        cerr << "致命错误: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}