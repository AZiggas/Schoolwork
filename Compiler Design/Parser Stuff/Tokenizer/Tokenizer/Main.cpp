
#include "Token.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<regex>
#include<map>
#include<set>

using namespace std;

vector<string> splitVec(string s) {
	vector<string> splitSet;
	size_t splitNum = s.find(" ");
	while (splitNum != string::npos) {
		splitSet.push_back(s.substr(0, splitNum));
		s.erase(0, splitNum + 1);
		splitNum = s.find(" ");
	}
	splitSet.push_back(s);
	return splitSet;
}

void main() {
	string curLine;
	ifstream tokFile ("Text.txt");

	//Stuff for TERMINALS
	vector<pair<string, regex>> terminals;
	bool terminalFlag = true;
	while (getline(tokFile, curLine) && terminalFlag == true) {
		if (terminalFlag == true && curLine.length() == 0) {
			terminalFlag = false;
			break;
		}

		size_t splitNum = curLine.find(" -> ");

		//Insert into vector
		terminals.emplace_back(curLine.substr(0, splitNum), regex(curLine.substr(splitNum + 4), regex::icase|regex::optimize|regex::nosubs));

		/*
		//If the first value in the pair matches the terminal name, set the iterator.
		vector<pair<string, string>>::iterator it = find_if(terminals.begin(), terminals.end(), [terminalName](pair <string, string> const &b) {
			return b.first == terminalName;


		});
		

		//In the case there isn't one do this.
		if (it == terminals.end()) {

			terminals.emplace_back(terminalName, line.substr(splitNum + 4));
		}
		else {
			it->second = (line.substr(splitNum + 4));
		}
		*/
	}

	//Stuff for NONTERMINALS
	map<string, vector<string>> nonTerminals;
	map<string, vector<string>>::iterator NT_it;
	while (getline(tokFile, curLine)) {
		if (curLine.length() != 0) {
			size_t splitNum = curLine.find(" -> ");
			string NTString = curLine.substr(0, splitNum);
			string productions = curLine.substr(splitNum + 4);
			vector<string> P;
			size_t pipeNum = productions.find(" | ");
			while (pipeNum != string::npos) {
				P.push_back(productions.substr(0, pipeNum));
				productions.erase(0, pipeNum + 3);
				pipeNum = productions.find(" | ");
			}
			P.push_back(productions);

			if (nonTerminals.find(NTString) == nonTerminals.end()) {
				nonTerminals[NTString] = P;
			}
			else {
				for (int i = 0; i < P.size(); i++){
					nonTerminals.find(NTString)->second.push_back(P[i]);
				}
			}
		}
	}
	tokFile.close();

	/*
	for (pair<string, string> temps : terminals) {
		cout << temps.first << " : " << temps.second << endl;
	}

	cout << endl;
	*/

	terminals.emplace_back("NEWLINE", "\\n");
	terminals.emplace_back("WHITESPACE", "\\s");

	//Stuff for TOKENIZING
	ifstream inFile("Input.txt");
	vector<Token> tokenList;

	try {
		if (inFile.is_open()) {
			stringstream buff;
			buff << inFile.rdbuf();
			string content(buff.str());

			bool matched = true;
			smatch match;
			int line = 1;

			while (matched) {
				for (pair<string, regex> symbol : terminals) {
					if (regex_search(content, match, symbol.second, regex_constants::match_flag_type::match_continuous)) {
						string lexeme = match[0];
						content.erase(content.find(lexeme), lexeme.length());

						if (symbol.first != "WHITESPACE") {
							if (symbol.first == "NEWLINE") {
								cout << endl;
								line++;
							}
							else {
								tokenList.push_back(Token(symbol.first, match.str(0), line));
								cout << symbol.first << " ";
							}
						}
						matched = true;
						break;
					}
					else {
						matched = false;
					}
				}
			}
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
		
	}
	inFile.close();
	
	/*
	int tempLine = 1;
	for (Token tok : tokenList) {
		if (tok.getLine() > tempLine) {
			tempLine = tok.getLine();
			cout << endl;
		}
		cout << tok.getSym() + " ";
	}
	cout << endl;
	*/


	//Setting up NULLABLES
	set<string> nullable;
	bool done = false;
	while (done == false) {
		//cout << "BOOM" << endl;
		done = true;
		//cout << "SHAKA" << endl;
		NT_it = nonTerminals.begin();
		while (NT_it != nonTerminals.end()) {
			if (nullable.find(NT_it->first) == nullable.end()) {
				for (string p : NT_it->second) {
					for (string product : splitVec(p)) {
						if (product != "lambda") {
							continue;
						}
						else {
							//cout << "LAKA" << endl;
							done = false;
							nullable.insert(NT_it->first);
						}
					}
				}
			}
			NT_it++;
		}
	}

	
	//Setting up FIRST
	map<string, set<string>> first;
	for (pair<string,regex> t : terminals) {
		first[t.first] = {t.first};
	}

	done = false;
	while (done == false) {
		//cout << "BOOM" << endl;
		done = true;
		//cout << "SHAKA" << endl;
		NT_it = nonTerminals.begin();
		while (NT_it != nonTerminals.end()) {
			for (string p : NT_it->second) {
				for (string x : splitVec(p)) {
					cout << x << endl;
					if (nullable.find(x) == nullable.end()) {
						break;
					}
					else {
						done = false;
						first[NT_it->first].insert(first[x].begin(), first[x].end());
					}
				}
				if (done == true) {
					break;
				}
			}
			NT_it++;
		}
	}
	cout << "Finished IT" << endl;
	//system("PAUSE");
}