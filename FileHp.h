#pragma once
#include<windows.h>
#include<Qstring>
class FileHp
{
public:
	~FileHp() {};
	FileHp(const FileHp&) = delete;
	FileHp& operator=(const FileHp&) = delete;
	static FileHp& get_instance() {
		static FileHp instance;
		return instance;
	}
	bool exist(const char* Path);
	bool exist(QString Path);
private:
	FileHp() {

	}
};

