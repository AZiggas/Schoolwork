
#include "Token.h"
#include "Item.h"
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

bool allNullable(vector<string> P, set<string> nullable) {
	for (string x : P) {
		if (nullable.find(x) == nullable.end()) {
			return false;
		}
	}
	return true;
}

set<string>findFirst(vector<string> P, set<string> extra, set<string> nullable, map<string, set<string>> first) {
	set<string> s;
	for (string x : P) {
		s.insert(first[x].begin(), first[x].end());
		if (nullable.find(x) == nullable.end()) {
			return s;
		}
	}
	s.insert(extra.begin(),extra.end());
	return s;
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
								//cout << endl;
								line++;
							}
							else {
								tokenList.push_back(Token(symbol.first, lexeme, line));
								//cout << symbol.first << " ";
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
	//cout << endl;


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
						if (product.compare("lambda") != 0) {
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
	//WORKS WITH LAMBDA NOW
	//cout << "FIRST" << endl;
	map<string, set<string>> first;
	for (pair<string,regex> t : terminals) {
		if ((t.first.compare("WHITESPACE") != 0) & (t.first.compare("NEWLINE") != 0)) {
			//cout << t.first << endl;
			first[t.first] = { t.first };
		}
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
					//cout << x << endl;
					for (string tmp : first[x]) {
						if (first[NT_it->first].find(tmp) == first[NT_it->first].end()) {
							done = false;
							first[NT_it->first].insert(tmp);
						}
					}
					if (nullable.find(x) == nullable.end()) {
						break;
					}
				}
			}
			NT_it++;
		}
	}
	//cout << first["WHERE"].size() << endl;
	

	//Setting up FOLLOW
	//cout << "FOLLOW" << endl;
	map<string, set<string>> follow;
	for (pair<string, vector<string>> t : nonTerminals) {
		follow[t.first] = {};
	}

	follow["S"].insert("$");
	done = false;
	int loopCount = 0;
	while (done == false) {
		done = true;
		for(pair<string,vector<string>> N : nonTerminals) {
			for (string p : N.second) {
				vector<string> productions = splitVec(p);
				for (int i = 0; i < productions.size();) {
					string x = productions[i];
					if (nonTerminals.find(x) != nonTerminals.end()) {
						bool breakFlag = false;
						for (int j = i + 1;  j < productions.size();) {
							string y = productions.at(j);
							
							for (string tmp : first[y]) {
								if (follow[x].find(tmp) == follow[x].end()) {
									done = false;
									follow[x].insert(tmp);
								}
							}
							if (nullable.find(y) == nullable.end()) {
								breakFlag = true;
								break;
							}
							j++;
						}
						if (breakFlag == false) {
							for (string tmp : follow[N.first]) {
								if (follow[x].find(tmp) == follow[x].end()) {
									done = false;
									follow[x].insert(tmp);
								}
							}
						}
					}
					i++;
				}
			}
		}
		if (loopCount < 1) {
			loopCount++;
			done = false;
		}

	}

	map<pair<int, int>, set<Item>>T;
	
	//Column = dot position.
	//Row = 
	//Scan moves column +1, row is same.
	//Predict. Item on first column it would go to the row that is labeled 1. So it would look like <column, column>, like <0,0> <1,1>, <2,2>
	//Complete. Say you completed, in column 2. You will add it to table[k][column] of the original item].
	for (size_t i = 0; i <= tokenList.size(); i++) {
		for (int j = 0; j <= i; j++) {
			T[pair<int,int>((int)i,j)] = {};
		}
	}
	
	Item init = Item("S'", set<string>{"S"}, 0);
	T[pair<int, int>(0, 0)] = { init };

	done = false;
	while (done == false) {
		done = true;
		for (size_t i = 0; i <= tokenList.size(); i++) {
			for (int j = 0; j <= i; j++) {
				for (Item item : T[pair<int, int>(static_cast<int>(i), j)]) {
					string tmp = *std::next(item.rhs.begin(), item.dpos);
					for(pair<string,regex> t : terminals) {
						if (t.first == tmp) {
							if (tokenList[static_cast<int>(i)].getSym() == tmp) { //SCAN
								Item item2 = Item(item.lhs, item.rhs, item.dpos + 1);
								if (T[pair<int, int>(static_cast<int>(i)+1, j)].find(item2) == T[pair<int, int>(static_cast<int>(i), j)].end()) {
									done = false;
									T[pair<int, int>(static_cast<int>(i)+1, j)].insert(item2);
								}
							}
							break;
						}
					}
					if (nonTerminals.find(tmp) != nonTerminals.end()) { //PREDICT

					}
				}
			}
		}
	}

	cout << "Finished IT" << endl;
	system("PAUSE");
}