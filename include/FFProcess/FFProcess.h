#pragma once
#include "pch.h"

#include<Windows.h>
#include <iostream>
#include <vector>

using namespace std;

class FFProcess {
public:
	DWORD lastError = 0;

	FFProcess();
	~FFProcess();
	int GetPidByName(const wchar_t*);
	int OpenProcessByPid();
	int GetBaseAddressByPid();

	template<typename ReadType>
	ReadType ReadGameMemory(SIZE_T addr);
	template<typename WriteType>
	WriteType WriteGameMemory(SIZE_T addr, WriteType writeData);
	template<typename ReadType>
	ReadType ReadGameMemory(SIZE_T addr, vector<SIZE_T>offsets);
	template<typename WriteType>
	WriteType WriteGameMemory(SIZE_T addr, WriteType writeData, vector<SIZE_T>offsets);
	template<typename ReadType>
	vector<ReadType> ReadGameMemory(SIZE_T addr, vector<SIZE_T>offsets, vector<SIZE_T>final_addr_offsets);

	template<typename ReadType>
	ReadType ReadGameMemory(SIZE_T addr, vector<SIZE_T>offsets, SIZE_T final_addr_offset);
	template<typename WriteType>
	WriteType WriteGameMemory(SIZE_T addr, WriteType writeData, vector<SIZE_T>offsets, SIZE_T final_addr_offset);

	float GetActiveItemRotation();
	int SetActiveItemRotation(float r);

	vector<float> GetActiveItemPos();
	int SetActiveItemX(float x);
	int SetActiveItemY(float y);
	int SetActiveItemZ(float z);
	int SetActivePos(vector<float> pos);

	int GetItemCount();
	int GetItemPageCount();
	int IsWarehouse();
	vector<int> GetItemList();

	int InjectPlaceAnywhere();
	int reInjectPlaceAnywhere();

	int InjectBlue();
	int reInjectBlue();

	int GetAllItemPos();
	vector<vector<float> > GetAllItemPos(int a);
	int SetAllItemPos(vector<vector<float> >pos);
	//sj
	int reInjectSj();
	int reInjectSj2();
	int reInjectSj3();
	int InjectSjWithPos(float x, float y, float z);
	int InjectSjWithPos2(float x, float y, float z);
	int InjectSjWithPos3(float x, float y, float z);
	
	//getAll --- setAll
	vector<vector<float> > GetAllItemPos(int a, int t2, int t0, int t);
	int SetAllItemPos(vector<vector<float> >pos, int t2, int t0, int t);

	//rotation
	vector<vector<float> > GetAllItemPosRotation(int a, int t2, int t0, int t);
	int SetAllItemPosRotation(vector<vector<float> >pos, int t2, int t0, int t);


	DWORD GetPid();
	SIZE_T GetBaseAdd();

private:
	DWORD pid = 0;
	SIZE_T baseAdd = 0;
	HWND window;
	HANDLE hProcess = NULL;

};



/*-------------------------template-------------------------*/

template<typename ReadType>
ReadType FFProcess::ReadGameMemory(SIZE_T addr) {
	ReadType buf;
	SIZE_T readSz;
	if (this->hProcess == NULL) {
		cout << "[ERROR] ReadGameMemory failed to open process" << endl;
		return NULL;
	}
	ReadProcessMemory(this->hProcess, (LPCVOID)addr, &buf, sizeof(ReadType), &readSz);
	if (readSz != sizeof(ReadType)) {
		cout << "[WARN] ReadGameMemory readsize is different from ReadType size" << endl;
	}
	return buf;
}

template<typename WriteType>
WriteType FFProcess::WriteGameMemory(SIZE_T addr, WriteType writeData) {
	SIZE_T readSz;
	if (this->hProcess == NULL) {
		cout << "[ERROR] WriteGameMemory failed to open process" << endl;
		return NULL;
	}
	WriteProcessMemory(this->hProcess, (LPVOID)addr, &writeData, sizeof(WriteType), &readSz);
	if (readSz != sizeof(WriteType)) {
		cout << "[WARN] WriteGameMemory readsize is different from WriteType size" << endl;
	}
	return writeData;
}

template<typename ReadType>
ReadType FFProcess::ReadGameMemory(SIZE_T addr, vector<SIZE_T>offsets) {
	int offset_size = offsets.size();
	ReadType buf;
	SIZE_T dwSize;
	SIZE_T value1;
	SIZE_T value2;
	//SIZE_T readSz;
	//ReadProcessMemory(this->hProcess,(LPVOID)addr, &value1, sizeof(SIZE_T), &dwSize);
	value1 = addr;

	cout << "ReadProcessMemory addr1 " << value1 << endl;

	for (int i = 0; i < offset_size - 1; i++) {
		SIZE_T offset = offsets[i];
		ReadProcessMemory(this->hProcess, (LPVOID)(value1 + offset), &value2, sizeof(SIZE_T), &dwSize);
		value1 = value2;
	}
	SIZE_T final_add = value1 + offsets.back();
	ReadProcessMemory(this->hProcess, (LPVOID)final_add, &buf, sizeof(buf), &dwSize);

	cout << "ReadProcessMemory final_add " << final_add << endl;

	if (dwSize != sizeof(ReadType)) {
		cout << "[WARN] ReadGameMemory readsize is different from ReadType size" << endl;
	}
	return buf;
}

template<typename WriteType>
WriteType FFProcess::WriteGameMemory(SIZE_T addr, WriteType writeData, vector<SIZE_T>offsets) {
	int offset_size = offsets.size();
	SIZE_T dwSize;
	SIZE_T value1;
	SIZE_T value2;
	//SIZE_T readSz;
	//ReadProcessMemory(this->hProcess,(LPVOID)addr, &value1, sizeof(SIZE_T), &dwSize);
	value1 = addr;
	for (int i = 0; i < offset_size - 1; i++) {
		SIZE_T offset = offsets[i];
		ReadProcessMemory(this->hProcess, (LPVOID)(value1 + offset), &value2, sizeof(SIZE_T), &dwSize);
		value1 = value2;
	}
	SIZE_T final_add = value1 + offsets.back();
	WriteProcessMemory(this->hProcess, (LPVOID)final_add, &writeData, sizeof(writeData), &dwSize);
	if (dwSize != sizeof(WriteType)) {
		cout << "[WARN] WriteGameMemory dwsize is different from WriteType size" << endl;
	}
	return writeData;
}

template<typename ReadType>
vector<ReadType> FFProcess::ReadGameMemory(SIZE_T addr, vector<SIZE_T>offsets, vector<SIZE_T>final_addr_offsets) {
	int read_count = final_addr_offsets.size();
	vector<ReadType> return_v;

	int offset_size = offsets.size();
	ReadType buf;
	SIZE_T dwSize;
	SIZE_T value1;
	SIZE_T value2;
	value1 = addr;
	for (int i = 0; i < offset_size - 1; i++) {
		SIZE_T offset = offsets[i];
		ReadProcessMemory(this->hProcess, (LPVOID)(value1 + offset), &value2, sizeof(SIZE_T), &dwSize);
		value1 = value2;
	}
	SIZE_T final_add = value1 + offsets.back();

	for (int i = 0; i < read_count; i++) {
		ReadProcessMemory(this->hProcess, (LPVOID)(final_add + final_addr_offsets[i]), &buf, sizeof(buf), &dwSize);
		return_v.push_back(buf);
	}

	if (dwSize != sizeof(ReadType)) {
		cout << "[WARN] ReadGameMemory readsize is different from ReadType size" << endl;
	}
	return return_v;
}

template<typename WriteType>
WriteType FFProcess::WriteGameMemory(SIZE_T addr, WriteType writeData, vector<SIZE_T>offsets, SIZE_T final_addr_offset) {
	int offset_size = offsets.size();
	SIZE_T dwSize;
	SIZE_T value1;
	SIZE_T value2;
	//SIZE_T readSz;
	//ReadProcessMemory(this->hProcess,(LPVOID)addr, &value1, sizeof(SIZE_T), &dwSize);
	value1 = addr;
	for (int i = 0; i < offset_size - 1; i++) {
		SIZE_T offset = offsets[i];
		ReadProcessMemory(this->hProcess, (LPVOID)(value1 + offset), &value2, sizeof(SIZE_T), &dwSize);
		value1 = value2;
	}
	SIZE_T final_add = value1 + offsets.back();
	WriteProcessMemory(this->hProcess, (LPVOID)(final_add + final_addr_offset), &writeData, sizeof(writeData), &dwSize);
	if (dwSize != sizeof(WriteType)) {
		cout << "[WARN] WriteGameMemory dwsize is different from WriteType size" << endl;
	}
	return writeData;
}

template<typename ReadType>
ReadType FFProcess::ReadGameMemory(SIZE_T addr, vector<SIZE_T>offsets, SIZE_T final_addr_offset) {
	int offset_size = offsets.size();
	ReadType buf;
	SIZE_T dwSize;
	SIZE_T value1;
	SIZE_T value2;
	//SIZE_T readSz;
	//ReadProcessMemory(this->hProcess,(LPVOID)addr, &value1, sizeof(SIZE_T), &dwSize);
	value1 = addr;

	cout << "ReadProcessMemory addr1 " << value1 << endl;

	for (int i = 0; i < offset_size - 1; i++) {
		SIZE_T offset = offsets[i];
		ReadProcessMemory(this->hProcess, (LPVOID)(value1 + offset), &value2, sizeof(SIZE_T), &dwSize);
		value1 = value2;
	}
	SIZE_T final_add = value1 + offsets.back();
	ReadProcessMemory(this->hProcess, (LPVOID)(final_add + final_addr_offset), &buf, sizeof(buf), &dwSize);

	cout << "ReadProcessMemory final_add " << final_add << endl;

	if (dwSize != sizeof(ReadType)) {
		cout << "[WARN] ReadGameMemory readsize is different from ReadType size" << endl;
	}
	return buf;
}