#pragma once
#include <map>
#include <cstring>
#include "FFJson.h"
class CItemInfo
{
public:
	
	map<int, string> id2name;
	map<string, int> name2id;
	FFJson readJ;
	void update();
	string f_id2name(int);
};

