#include "AudioManager.h"
#include "Global.h"

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}

void AudioManager::cacheAudio()
{
	/*experimental::AudioEngine::preload(MUSIC_MENU);
	experimental::AudioEngine::preload(MUSIC_STAGE1);
	experimental::AudioEngine::preload(MUSIC_STAGE2);
	experimental::AudioEngine::preload(MUSIC_STAGE3);
	experimental::AudioEngine::preload(MUSIC_STAGE4);*/
	experimental::AudioEngine::preload(SOUND_DQDIE);
	experimental::AudioEngine::preload(SOUND_DQHIT);
	experimental::AudioEngine::preload(SOUND_DQSKILL1);
	experimental::AudioEngine::preload(SOUND_DQSKILL2);
	experimental::AudioEngine::preload(SOUND_DQSKILL3);
	experimental::AudioEngine::preload(SOUND_CLDIE);
	experimental::AudioEngine::preload(SOUND_CLHIT);
	experimental::AudioEngine::preload(SOUND_CLSKILL1);
	experimental::AudioEngine::preload(SOUND_CLSKILL2);
	experimental::AudioEngine::preload(SOUND_CLSKILL3);
	experimental::AudioEngine::preload(SOUND_TC1AT);
	experimental::AudioEngine::preload(SOUND_TC1DIE);
	experimental::AudioEngine::preload(SOUND_TC2AT);
	experimental::AudioEngine::preload(SOUND_TC2DIE);
	experimental::AudioEngine::preload(SOUND_HLB1AT);
	experimental::AudioEngine::preload(SOUND_HLB1DIE);
	experimental::AudioEngine::preload(SOUND_HLB2AT);
	experimental::AudioEngine::preload(SOUND_HLB2DIE);
	experimental::AudioEngine::preload(SOUND_MCAT);
	experimental::AudioEngine::preload(SOUND_MCJUMP);
	experimental::AudioEngine::preload(SOUND_TOONGDIE);
	experimental::AudioEngine::preload(SOUND_BOSS1CHEM);
	experimental::AudioEngine::preload(SOUND_BOSS1DIE);
	experimental::AudioEngine::preload(SOUND_BOSS1SKILL);
	experimental::AudioEngine::preload(SOUND_COIN);
	experimental::AudioEngine::preload(SOUND_COINBULLION);
	experimental::AudioEngine::preload(SOUND_COINBAG);
	experimental::AudioEngine::preload(SOUND_FAIL);
	experimental::AudioEngine::preload(SOUND_WIN);

}

void AudioManager::playSound(string keysound)
{
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	bool checkSound = ref->getBoolForKey(KEY_IS_SOUND,true);
	if (checkSound) {
		experimental::AudioEngine::play2d(keysound);
	}
}

int AudioManager::playSoundForever(string keysound)
{
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	bool checkSound = ref->getBoolForKey(KEY_IS_SOUND, true);
	if (checkSound) {
		return experimental::AudioEngine::play2d(keysound, true);
	}
	return NULL;
}

void AudioManager::stopSoundForever(int keysound)
{
	experimental::AudioEngine::stop(keysound);
}

int AudioManager::playMusic(string keysound)
{
	AudioManager::stopMusic();
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	bool checkSound = ref->getBoolForKey(KEY_IS_MUSIC, true);
	//log("check sound: %d: ", checkSound);
	if (checkSound) {
		
		int a = experimental::AudioEngine::play2d(keysound,true);
		ref->setIntegerForKey(KEY_VALUE_MUSIC, a);
		return a;
	}
	return 0;
}

void AudioManager::stopMusic()
{
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	int checkSound = ref->getIntegerForKey(KEY_VALUE_MUSIC);
	//if (checkSound <0 0) {
	experimental::AudioEngine::stop(checkSound);
	//}
}

void AudioManager::stopSoundandMusic()
{
	experimental::AudioEngine::stopAll();
}
