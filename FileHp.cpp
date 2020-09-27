#include "FileHp.h"

#include<io.h>

bool FileHp::exist(const char* Path) {
	if ((_access(Path, 0)) == -1) {
		return false;
	}
	return true;
}

bool FileHp::exist(QString Path) {
	QByteArray ba2;
	ba2.append(Path);
	const char* c2 = ba2.data();
	if ((_access(c2, 0)) == -1) {
		return false;
	}
	return true;
}
