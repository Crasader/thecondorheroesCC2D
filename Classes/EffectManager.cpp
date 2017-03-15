#include "EffectManager.h"

EffectManager* EffectManager::effectManager;

EffectManager::EffectManager()
{
	
}

EffectManager::~EffectManager()
{
	slashBreak = nullptr;
	smokeJumpX2 = nullptr;
	smokeLanding = nullptr;
	smokeRun = nullptr;
}

EffectManager * EffectManager::getInstance()
{
	if (effectManager == nullptr)
		effectManager = new EffectManager();
	return effectManager;
}

void EffectManager::createWithFile(float scale)
{
	slashBreak = new SkeletonAnimation("Effect/slash-break.json", "Effect/slash-break.atlas", scale / 2);
	smokeJumpX2 = new SkeletonAnimation("Effect/smoke-jumpx2.json", "Effect/smoke-jumpx2.atlas", scale / 2);
	smokeLanding = new SkeletonAnimation("Effect/smoke-landing.json", "Effect/smoke-landing.atlas", scale / 2);
	smokeRun = new SkeletonAnimation("Effect/smoke-run.json", "Effect/smoke-run.atlas", scale);

	slashBreak->setVisible(false);		slashBreak->update(0.0f);
	smokeJumpX2->setVisible(false);		smokeJumpX2->update(0.0f);
	smokeLanding->setVisible(false);	smokeLanding->update(0.0f);
	smokeRun->setVisible(false);		smokeRun->update(0.0f);
}

void EffectManager::slashBreakAni()
{
	slashBreak->clearTracks();
	slashBreak->addAnimation(0, "slash-break", false);
	slashBreak->setToSetupPose();
}

void EffectManager::smokeJumpX2Ani()
{
	smokeJumpX2->clearTracks();
	smokeJumpX2->addAnimation(0, "smoke-jumpx2", false);
	smokeJumpX2->setToSetupPose();
}

void EffectManager::smokeLandingAni()
{
	smokeLanding->clearTracks();
	smokeLanding->addAnimation(0, "smoke-landing", false);
	smokeLanding->setToSetupPose();
}

void EffectManager::smokeRunAni()
{
	smokeRun->clearTracks();
	smokeRun->addAnimation(0, "smoke-run", true);
	smokeRun->setToSetupPose();
}

