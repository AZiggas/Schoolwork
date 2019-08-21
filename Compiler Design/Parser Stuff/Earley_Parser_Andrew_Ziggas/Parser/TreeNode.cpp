#include "TreeNode.h"
#include "Token.h"

TreeNode::TreeNode(string symbol, string token) {
	this->symbol = symbol;
	this->token = token;
	this->children = {};
}