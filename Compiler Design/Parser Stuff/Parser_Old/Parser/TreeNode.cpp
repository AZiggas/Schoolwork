#include "TreeNode.h"
#include "Token.h"

TreeNode::TreeNode(string symbol) {
	this->symbol = symbol;
	//this->token = token;
	this->children = {};
}