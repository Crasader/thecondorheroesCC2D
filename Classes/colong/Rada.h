#pragma once
#ifndef __RADA_H__
#define __RADA_H__

#include "B2Sprite.h"

class Rada : public B2Sprite {
public:
	Rada();
	~Rada();

	static Rada * create(string p_sFileName);
	virtual void initCirclePhysic(b2World *p_pWorld, Point p_ptPosition);
};

#endif //__RADA_H__