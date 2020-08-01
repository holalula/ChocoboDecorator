#include "CItemInfo.h"

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
				//
				this->id2name.insert(make_pair(intValue, strKey));
				this->name2id.insert(make_pair(strKey, intValue));
			}
		}
	}
}


