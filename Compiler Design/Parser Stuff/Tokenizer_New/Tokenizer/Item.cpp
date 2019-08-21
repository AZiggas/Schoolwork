#include "Item.h"

Item::Item(string lhs, set<string> rhs, int dotPos) {
	this->lhs = lhs;
	this->rhs = rhs;
	this->dpos = dotPos;
}