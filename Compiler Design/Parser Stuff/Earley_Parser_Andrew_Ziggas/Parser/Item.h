#pragma once
#include<string>
#include<vector>
#include<tuple>
#include<algorithm>

using namespace std;

enum source {
	Scan,
	Predict,
	Complete,
	Initial
};


class Item {
	public:
		Item(string lhs, vector<string> rhs, int dotPos, int processNum,string state, int column, int row, Item* Source, Item* Partial);
		Item(string lhs, vector<string> rhs, int dotPos, int processNum, string state, int column, int row, Item* Source);
		Item(string lhs, vector<string> rhs, int dotPos, int processNum, string state, int column, int row);
		Item();
		string lhs;
		vector<string> rhs;
		int dpos;
		int processNum;
		int column;
		int row;
		string state;
		//vector<Item *> how;
		Item *Source;
		Item *Partial;

		bool operator== (const Item &right) const{
			if (lhs.compare(right.lhs) != 0) {
				return false;
			}
			if (rhs != right.rhs || rhs.size() != right.rhs.size()) {
				return false;
			}
			if (dpos != right.dpos) {
				return false;
			}
			if (state != right.state) {
				return false;
			}
			return true;
		}

		bool operator!= (const Item &right) const {
			if (lhs == right.lhs) {
				return false;
			}
			if (rhs == right.rhs && rhs.size() == right.rhs.size()) {
				return false;
			}
			if (dpos == right.dpos) {
				return false;
			}
			if (state == right.state) {
				return false;
			}
			return true;
		}
		
		bool operator< (const Item &right) const{
			vector<string> a = rhs;
			vector<string> b = right.rhs;
			sort(a.begin(), a.end());
			sort(b.begin(), b.end());
			//return (a == b);
			/*
			if (lhs == right.lhs && a == b) {
				if (dpos == right.dpos) {
					if (state != right.state) {
						return (processNum < right.processNum);
					}
					return (dpos < right.dpos);
				}
				return (dpos < right.dpos);
			}
			else if (lhs == right.lhs) {
				if (rhs.size() == 0 || right.rhs.size() == 0) {
					return (processNum < right.processNum);
				}
				if (dpos == right.dpos) {
					if (state != right.state) {
						return (processNum < right.processNum);
					}
					return(a < b);
				}
				return(a < b);
			}
			else if (a == b) {
				if (rhs.size() == 0 || right.rhs.size() == 0) {
					return (processNum < right.processNum);
				}
				if (dpos == right.dpos) {
					if (state != right.state) {
						return (processNum < right.processNum);
					}
					return(processNum < right.processNum);
				}
				return(processNum < right.processNum);
			}
			if (rhs.size() == 0 || right.rhs.size() == 0) {
				return (processNum < right.processNum);
			}
			else {
				return (a < b);
			}
			*/

			if (lhs.compare(right.lhs) == 0) {
				if (a == b && a.size() == b.size()) {
					if (dpos == right.dpos) {
						if (state == right.state) {
							return (1 < 1);
						}
						return (state < right.state);
					}
					return (dpos < right.dpos);
				}
				return (a < b);
			}
			if (lhs.compare(right.lhs) != 0) {
				if (a == b && a.size() == b.size()) {
					if (dpos == right.dpos) {
						if (state == right.state) {
							return (1 < 1);
						}
						return (state < right.state);
					}
					return (dpos < right.dpos);
				}
				return (lhs < right.lhs);
			}
			
		}	
};