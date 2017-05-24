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
	void initPhysic(b2World *world, float radius, Point posHead1, Point posHead2);
	void updateDragons(Point posHead1, Point posHead2);
};

#define SongLong QT_SongLongXuatThuy

#endif // __QT_SONG_LONG_XUAT_THUY_H__