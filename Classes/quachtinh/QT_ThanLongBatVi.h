#ifndef __QT_THAN_LONG_BAT_VI_H__
#define __QT_THAN_LONG_BAT_VI_H__

#include "B2Sprite.h"


class QT_ThanLongBatVi : public B2Sprite
{

public:
	QT_ThanLongBatVi();
	~QT_ThanLongBatVi();

	static QT_ThanLongBatVi* create();
	virtual void initBoxPhysic(b2World *world, Point pos);
	void runAni();

protected:
	CC_SYNTHESIZE(bool, isAdded, IsAdded);
	
};

#define ThanLong QT_ThanLongBatVi

#endif // __QT_THAN_LONG_BAT_VI_H__