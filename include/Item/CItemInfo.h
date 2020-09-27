#pragma once
#include <map>
#include <cstring>
#include "FFJson.h"
class CItemInfo
{
public:
	
	map<int, string> id2name;
	map<string, int> name2id;
	map<int, string> id2name2;
	map<string, int> name2id2;

	map<int, int> memo2icon;

	FFJson readJ;
	FFJson readJ2;
	void update();
	string f_id2name(int);
};

