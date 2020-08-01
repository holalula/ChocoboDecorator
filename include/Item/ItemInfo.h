#pragma once
#include "pch.h"

#include <map>
#include <string>

using namespace std;

static class ItemInfo
{
public:
	ItemInfo();
	static void UpdateItemInfo();
	static map<int, string> id2name;

private:

};

