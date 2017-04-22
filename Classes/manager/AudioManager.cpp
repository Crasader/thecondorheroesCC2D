#include "AudioManager.h"
#include "Global.h"

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
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
	int checkSound = ref->getIntegerForKey(KEY_IS_MUSIC, -1);
	log("check sound: %d: ", checkSound);
	if (checkSound <= 0) {
		
		int a = experimental::AudioEngine::play2d(keysound,true);
		ref->setIntegerForKey(KEY_IS_MUSIC, a);
		return a;
	}
	return 0;
}

void AudioManager::stopSoundForever()
{
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	int checkSound = ref->getIntegerForKey(KEY_IS_MUSIC);
	//if (checkSound <0 0) {
		experimental::AudioEngine::stop(checkSound);
	//}
}
