#pragma once
#include "Token.h"
#include "Item.h"
using namespace std;

class TreeNode {
	public:
		TreeNode(string symbol);

		string symbol;
		//string token;
		vector<TreeNode> children;

};