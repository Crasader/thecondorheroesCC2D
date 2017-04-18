#ifndef __DQ_TOAN_CHAN_KIEM_PHAP_H__
#define __DQ_TOAN_CHAN_KIEM_PHAP_H__

#include "DQ_TieuHonChuong.h"

class DQ_ToanChanKiemPhap : public DQ_TieuHonChuong
{

public:
	DQ_ToanChanKiemPhap();
	~DQ_ToanChanKiemPhap();


	static DQ_ToanChanKiemPhap* create();
	virtual void initCirclePhysic(b2World *world, Point pos);
};

#define ToanChanKiemPhap DQ_ToanChanKiemPhap

#endif //__DQ_TOAN_CHAN_KIEM_PHAP_H__