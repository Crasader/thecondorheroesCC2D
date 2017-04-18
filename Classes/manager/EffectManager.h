#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

#include "cocos2d.h"
#include <string>
#include <spine/spine-cocos2dx.h>

USING_NS_CC;
using namespace spine;

class EffectManager
{
public:

	EffectManager();
	~EffectManager();

protected:
	CC_SYNTHESIZE(SkeletonAnimation *, slashBreak, SlashBreak);
	CC_SYNTHESIZE(SkeletonAnimation *, smokeJumpX2, SmokeJumpX2);
	CC_SYNTHESIZE(SkeletonAnimation *, smokeLanding, SmokeLanding);
	CC_SYNTHESIZE(SkeletonAnimation *, smokeRun, SmokeRun);
	CC_SYNTHESIZE(SkeletonAnimation *, reviveMe, ReviveMe);
	//CC_SYNTHESIZE(SkeletonAnimation*, exxp, Exxp);

private:
	static EffectManager *effectManager;

public:
	static EffectManager* getInstance();


	void createWithFile(float scale);

	void slashBreakAni();
	void smokeJumpX2Ani();
	void smokeLandingAni();
	void smokeRunAni();
	void reviveAni();
};

#define EM EffectManager::getInstance()

#endif // __EFFECT_MANAGER_H__