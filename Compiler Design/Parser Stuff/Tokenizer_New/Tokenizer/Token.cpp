#include "Token.h"

using namespace std;

Token::Token(string sym, string lex, int line) {
	this->symbol = sym;
	this->lexeme = lex;
	this->lineNum = line;
}

string Token::getSym() {
	return this->symbol;
}

string Token::getLex() {
	return this->lexeme;
}

int Token::getLine() {
	return this->lineNum;
}