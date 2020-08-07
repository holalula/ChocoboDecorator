#pragma  once
#include "pch.h"
#include <vector>
#include <windows.h>

using namespace std;

static class OffsetMgr {
public:
	static SIZE_T ActiveItemPosition0;
	static SIZE_T ActiveItemPosition1;
	static vector<SIZE_T> ActiveItemPosition2;
	static vector<SIZE_T> ActiveItemRotation2;
	static SIZE_T BlueInject;
	static SIZE_T YellowInject;

	static SIZE_T ItemCount;
	static vector<SIZE_T> ItemCount2;
	static vector<SIZE_T> ItemCount3;
	static SIZE_T IsWarehouseOffset;
	static SIZE_T ItemCount_List;

	static int OutputOffsetsJSON(const char* FilePath);
};