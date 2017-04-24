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
	AudioManager::stopSoundForever();
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	bool checkSound = ref->getBoolForKey(KEY_IS_MUSIC, true);
	log("check sound: %d: ", checkSound);
	if (checkSound) {
		
		int a = experimental::AudioEngine::play2d(keysound,true);
		ref->setIntegerForKey(KEY_VALUE_MUSIC, a);
		return a;
	}
	return 0;
}

void AudioManager::stopSoundForever()
{
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	int checkSound = ref->getIntegerForKey(KEY_VALUE_MUSIC);
	//if (checkSound <0 0) {
	experimental::AudioEngine::stop(checkSound);
	//}
}
