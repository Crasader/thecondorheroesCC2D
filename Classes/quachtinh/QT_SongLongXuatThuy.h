#ifndef __QT_SONG_LONG_XUAT_THUY_H__
#define __QT_SONG_LONG_XUAT_THUY_H__

#include "B2Skeleton.h"



class QT_SongLongXuatThuy : public SkeletonAnimation
{
protected:
	CC_SYNTHESIZE(b2Body*, head_1, Head_1);
	CC_SYNTHESIZE(b2Body*, head_2, Head_2);

public:
	QT_SongLongXuatThuy(string jsonFile, string atlasFile, float scale);
	static QT_SongLongXuatThuy* create(string jsonFile, string atlasFile, float scale);
	void initPhysic(b2World *world, float radius);
	void updateDragons();
	void changeBodyCategoryBits(uint16 mask);

private:
	Point getBoneLocation(string name);
};

#define SongLong QT_SongLongXuatThuy

#endif // __QT_SONG_LONG_XUAT_THUY_H__