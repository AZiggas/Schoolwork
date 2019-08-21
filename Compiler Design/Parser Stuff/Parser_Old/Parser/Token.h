#pragma once
#include <string>
using namespace std;

class Token
{
private:
	string symbol, lexeme;
	int lineNum;
public:
	Token(string sym, string lex, int line);
	string getSym();
	string getLex();
	int getLine();
};

