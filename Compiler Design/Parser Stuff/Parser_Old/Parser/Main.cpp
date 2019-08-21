#include "Token.h"
#include "Item.h"
#include "TreeNode.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<regex>
#include<map>
#include<set>
#include<chrono>
#include<ctime>

using namespace std;

void makeTree(Item item, TreeNode Node, vector<Token>tokenList) {
	if (item.state == "Initial") {
		Node.symbol = "S'";
		return;
	}
	else if (item.state == "Scan") {
		cout << "Scan" << endl;
		TreeNode temp = TreeNode(item.rhs.at(item.dpos - 1));
		Node.children.insert(Node.children.begin(), temp);
		return makeTree(*item.Source, Node, tokenList);
	}
	else if (item.state == "Predict") {
		cout << "Predict" << endl;
		return;
	}
	else if (item.state == "Complete") {
		cout << "Complete" << endl;
		//cout << "STUFF HERE Too: " << item.Source->dpos << endl;
		Item complete = *item.Source;
		Item partial = *item.Partial;
		TreeNode Q = TreeNode(complete.lhs);
		Node.children.insert(Node.children.begin(), Q);
		return makeTree(partial, Node, tokenList);
	}
	return;
}


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
#define RESET_TIME_TRACKER 		startMs = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
#define PRINT_ELAPSED_TIME 		endMs = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());printf("~Process Time: %fs\n", (float)(endMs.count() - startMs.count()) / 1000000.0f);

int main(int argc, char* args[]) {
	std::chrono::microseconds startMs;
	std::chrono::microseconds endMs;
	std::chrono::microseconds startTimeMs = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	RESET_TIME_TRACKER;
	string curLine;
	ifstream tokFile(args[2]); //Grammar file

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
		terminals.emplace_back(curLine.substr(0, splitNum), regex(curLine.substr(splitNum + 4), regex::icase | regex::optimize | regex::nosubs));
	}
	cout << "Time it took to get terminals: ";
	PRINT_ELAPSED_TIME;
	RESET_TIME_TRACKER;
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
				for (int i = 0; i < P.size(); i++) {
					nonTerminals.find(NTString)->second.push_back(P[i]);
				}
			}
		}
	}
	tokFile.close();

	terminals.emplace_back("NEWLINE", "\\n");
	terminals.emplace_back("WHITESPACE", "\\s");

	cout << "Time it took to get nonterminals: ";
	PRINT_ELAPSED_TIME;
	RESET_TIME_TRACKER;

	//Stuff for TOKENIZING
	ifstream inFile(args[1]); //Input file
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

	cout << "Time it took to tokenize: ";
	PRINT_ELAPSED_TIME;
	RESET_TIME_TRACKER;
	//Setting up NULLABLES
	/*
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

	*/

	//Setting up FIRST
	//cout << "FIRST" << endl;
	/*
	map<string, set<string>> first;
	for (pair<string, regex> t : terminals) {
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
	
	*/


	//Setting up FOLLOW
	//cout << "FOLLOW" << endl;
	/*
	map<string, set<string>> follow;
	for (pair<string, vector<string>> t : nonTerminals) {
		follow[t.first] = {};
	}

	follow["S"].insert("$");
	done = false;
	int loopCount = 0;
	while (done == false) {
		done = true;
		for (pair<string, vector<string>> N : nonTerminals) {
			for (string p : N.second) {
				vector<string> productions = splitVec(p);
				for (int i = 0; i < productions.size();) {
					string x = productions[i];
					if (nonTerminals.find(x) != nonTerminals.end()) {
						bool breakFlag = false;
						for (int j = i + 1; j < productions.size();) {
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
	
	*/
	typedef set<Item> itemSet;
	map<pair<int, int>, itemSet>T;
	
	for (size_t i = 0; i <= tokenList.size(); i++) {
		for (int j = 0; j <= i; j++) {
			itemSet tmpSet = {};
			T[pair<int, int>(i, j)] = tmpSet;
		}
	}

	Item init = Item("Start", vector<string>{"S"}, 0, 0, "Initial", 0, 0);
	T[pair<int, int>(0, 0)].insert(init);
	int procNum = 1;
	//cout << "Earley Parse Table:" << endl;
	bool is_inFlag = false;
	bool done = false;
	int check = 0;
	bool wasInFlag = false;
	bool loopCheck = false;
	Item endItem;
	while (loopCheck == false && is_inFlag == false) {
		loopCheck = true;
		for (size_t i = 0; i <= tokenList.size(); i++) {
			//cout << "I: " << i << endl;
			done = false;
			while (done == false && is_inFlag == false) {
				done = true;
				for (int j = 0; j <= i; j++) {
					//cout << "I:J: " << i << " : " << j << endl;
					//cout << "Item Loop Start" << endl;
					for (Item item : T[pair<int, int>(i, j)]) {
						//cout << "COMPLETE" << endl;
						if (item.dpos == item.rhs.size() && i != 0) { //COMPLETE
							for (int k = 0; k <= j; k++) {
								//if (k == 29 && j == 29) {
								//	cout << "Hello" << endl;
								//}
								for (Item tempItem : T[pair<int, int>(j, k)]) {
									//if (tempItem.lhs == "optional-interjection") {
									//	cout << "Hello" << endl;
									//}
									if (tempItem.dpos != tempItem.rhs.size() && item.lhs == tempItem.rhs.at(tempItem.dpos)) {
										set<Item>::iterator it = T[pair<int, int>(i, j)].find(item);
										set<Item>::iterator it2 = T[pair<int, int>(j, k)].find(tempItem);
										
										Item newItem = Item(it2->lhs, it2->rhs, it2->dpos + 1, procNum, "Complete", i, k, &*it, &*it2);
										if (T[pair<int, int>(i, k)].find(newItem) == T[pair<int, int>(i, k)].end()) {
											loopCheck = false;
											done = false;
											procNum++;
											if (newItem.lhs == "Start" && it2->dpos == 1 && i == tokenList.size() && k == 0) {
												is_inFlag = true;
												wasInFlag = true;
												endItem = newItem;
											}
											//cout << i << " : " << k << "  " << newItem.lhs << "  " << newItem.dpos << "  COMPLETE" << endl;
											T[pair<int, int>(i, k)].insert(newItem);
										}
									}
								}
							}
						}
						//cout << "COMPLETE END" << endl;
						//cout << "SCAN" << endl;
						else if (i < tokenList.size() && tokenList.at(i).getSym().compare(item.rhs.at(item.dpos)) == 0) { //SCAN
							for (pair<string, regex> t : terminals) {
								if (t.first.compare(item.rhs.at(item.dpos)) == 0) {
									set<Item>::iterator it = T[pair<int, int>(i, j)].find(item);
									Item item2 = Item(it->lhs, it->rhs, item.dpos + 1, procNum, "Scan", i+1, j, &*it);
									if ((i+1 <= tokenList.size()) && T[pair<int, int>(i + 1, j)].find(item2) == T[pair<int, int>(i + 1, j)].end()) {
										loopCheck = false;
										done = false;
										procNum++;
										T[pair<int, int>(i + 1, j)].insert(item2);
										break;
									}
								}
							}
						}
						//cout << "SCAN END" << endl;
						//cout << "PREDICT" << endl;

						//GIT GUD YOUR LESS THAN COMPARATOR BE JANK
						else if ((nonTerminals.find(item.rhs.at(item.dpos))) != nonTerminals.end()) { //PREDICT
							for (string str : nonTerminals[item.rhs.at(item.dpos)]) {
								if (str == "lambda") {
									set<Item>::iterator it = T[pair<int, int>(i, j)].find(item);
									Item item2 = Item(it->rhs.at(it->dpos), {}, 0, procNum, "Predict", i, i, &*it);
									if (T[pair<int, int>(i, i)].find(item2) == T[pair<int, int>(i, i)].end()) {
										loopCheck = false;
										done = false;
										procNum++;
										T[pair<int, int>(i, i)].insert(item2);
									}
								}
								else {
									vector<string> split = splitVec(str);
									set<Item>::iterator it = T[pair<int, int>(i, j)].find(item);
									Item item2 = Item(it->rhs.at(it->dpos), split, 0, procNum, "Predict", i, i, &*it);
									//cout << i << endl;
									//cout << "FIND OTHER" << endl;
									if (T[pair<int, int>(i, i)].find(item2) == T[pair<int, int>(i, i)].end()) {
										loopCheck = false;
										done = false;
										procNum++;
										T[pair<int, int>(i, i)].insert(item2);
									}
									//cout << "FIND END" << endl;
								}
							}
						}
						//cout << "FIND END" << endl;
						//cout << "PREDICT END" << endl;
						if (is_inFlag) {
							break;
						}
					}
					//cout << "ITEM LOOP END" << endl;
					if (is_inFlag) {
						break;
					}
				}
			}
			if (is_inFlag) {
				break;
			}
		}
		//if (check < 100) {
		//	is_inFlag = false;
		//	loopCheck = false;
		//	check++;
		//}
	}

	cout << "Time it took to parse: ";
	PRINT_ELAPSED_TIME;
	RESET_TIME_TRACKER;

	cout << "Finished IT" << endl;
	std::chrono::microseconds endTimeMs = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	printf("Total Runtime: %fs\n", (float)(endTimeMs.count() - startTimeMs.count()) / 1000000.0f);


	//cout << "HELLO" << endl;
	if (is_inFlag) {
		cout << "The input is valid and is in the language\n\n" << endl;
		//TreeNode *startNode = new TreeNode("blah");
		//makeTree(endItem, *startNode, tokenList);
		//system("PAUSE");
		return 0;
	}
	else {
		cout << "The input is not valid and is not in the language\n\n" << endl;
		//system("PAUSE");
		return 1;
	}

}