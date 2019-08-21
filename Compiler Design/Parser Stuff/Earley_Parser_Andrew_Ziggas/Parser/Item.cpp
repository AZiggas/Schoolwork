#include "Item.h"

Item::Item(string lhs, vector<string> rhs, int dotPos, int processNum, string state, int column, int row, Item* Source, Item* Partial) {
	this->lhs = lhs;
	this->rhs = rhs;
	this->dpos = dotPos;
	this->processNum = processNum;
	this->state = state;
	this->column = column;
	this->row = row;
	this->Source = Source;
	this->Partial = Partial;
}

Item::Item(string lhs, vector<string> rhs, int dotPos, int processNum, string state, int column, int row, Item* Source) {
	this->lhs = lhs;
	this->rhs = rhs;
	this->dpos = dotPos;
	this->processNum = processNum;
	this->state = state;
	this->column = column;
	this->row = row;
	this->Source = Source;
}

Item::Item(string lhs, vector<string> rhs, int dotPos, int processNum, string state, int column, int row) {
	this->lhs = lhs;
	this->rhs = rhs;
	this->dpos = dotPos;
	this->processNum = processNum;
	this->state = state;
	this->column = column;
	this->row = row;
}
Item::Item() {
	
}