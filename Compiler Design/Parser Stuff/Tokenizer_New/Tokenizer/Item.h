#pragma once
#include<string>
#include<set>
#include<tuple>

using namespace std;

class Item {
public:
	operator bool() const {
		return false;
	}
	Item(string lhs, set<string> rhs, int dotPos);

	string lhs;
	set<string> rhs;
	int dpos;

};

//Settup enumerators for source. COMPLETE, SCAN, PREDICT, INITIAL
//Have a thing for the source of the item. productionSource = string, (int,int) for location in the table.