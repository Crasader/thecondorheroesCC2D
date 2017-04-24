#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include "cocos2d.h"
#include <string>
#include "AudioEngine.h"

USING_NS_CC;
using namespace std;
//using namespace experimental;

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	static void cacheAudio();
	static void playSound(string keysound);
	static int playSoundForever(string keysound);
	static void stopSoundForever();
};
#endif // __TANK_SOLDIER_H__