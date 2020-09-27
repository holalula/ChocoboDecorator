#include "CItemInfo.h"

#include <iostream>
#include <qDebug>


void CItemInfo::update() {
	readJ.ReadJsonFile("ItemInfo.json");
	id2name.erase(id2name.begin(), id2name.end());
	name2id.erase(name2id.begin(), name2id.end());
	for (Value::ConstMemberIterator itr = readJ.document.MemberBegin(); itr != readJ.document.MemberEnd(); itr++) {
		Value jkey;
		Value jvalue;
		Document::AllocatorType allocator;
		jkey.CopyFrom(itr->name, allocator);
		jvalue.CopyFrom(itr->value, allocator);
		if (jkey.IsString()) {
			if (jvalue.IsInt()) {
				string strKey = jkey.GetString();
				int intValue = jvalue.GetInt();

				this->id2name.insert(make_pair(intValue, strKey));
				this->name2id.insert(make_pair(strKey, intValue));
			}
		}
	}
	

	readJ2.ReadJsonFile("ItemInfo2.json");
	id2name2.erase(id2name2.begin(), id2name2.end());
	name2id2.erase(name2id2.begin(), name2id2.end());
	memo2icon.erase(memo2icon.begin(), memo2icon.end());
	for (Value::ConstMemberIterator itr = readJ2.document.MemberBegin(); itr != readJ2.document.MemberEnd(); itr++) {
		Value jkey;
		Value jvalue;
		Document::AllocatorType allocator;
		jkey.CopyFrom(itr->name, allocator);
		jvalue.CopyFrom(itr->value, allocator);
		if (jkey.IsString()) {
			if (jvalue.IsInt()) {
				string strKey = jkey.GetString();
				int intValue = jvalue.GetInt();
				//
				if (name2id.find(strKey)!=name2id.end()) {
					this->memo2icon.insert(make_pair(intValue, name2id[strKey]));
				}
				else {
					//std::cout <<"not found:"<<strKey<< endl;
				}
				this->id2name2.insert(make_pair(intValue, strKey));
				this->name2id2.insert(make_pair(strKey, intValue));
			}
		}
	}
}


