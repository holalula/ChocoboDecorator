#pragma once
#include "pch.h"

#include <string> //stod
#include <vector>
#include "document.h"
#include "stringbuffer.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include "writer.h"

using namespace std;
using namespace rapidjson;

class FFJson {
public:
	FFJson();
	~FFJson();

	MemoryPoolAllocator<>& GetAllocator();

	int AddMember(GenericStringRef<char> key, GenericStringRef<char> value);
	int AddMember(GenericStringRef<char> key, Document value);
	int AddMember(GenericStringRef<char> key, vector<GenericStringRef<char>> value);

	int RemoveMember(GenericStringRef<char> key);

	int ReadJsonFile(const char* FilePath);
	int WriteJsonFile(const char* FilePath);
	int PrintJsonString();
	int JsonTraversal();

	Document document;

private:
	string JsonToString(Value& valRoot);

};
