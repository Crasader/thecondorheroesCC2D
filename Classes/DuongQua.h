#ifndef __DUONG_QUA_H__
#define __DUONG_QUA_H__

#include "B2Skeleton.h"

USING_NS_CC;

using namespace spine;

class DuongQua : public B2Skeleton
{
public:

	DuongQua(string jsonFile, string atlasFile, float scale);
	static DuongQua* create(string jsonFile, string atlasFile, float scale);

protected:
	void update();

};
#endif // __DuongQua_H__