#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include <string>
#include <vector>

USING_NS_CC;
using namespace std;


// define
#define PI								3.1415926
#define PTM_RATIO						32


// bitmask
#define BITMASK_HERO					0x0001
#define BITMASK_ENEMY					0x0002
#define BITMASK_FLOOR					0x0004
#define BITMASK_WOODER					0x0008
#define BITMASK_COIN					0x0010
#define BITMASK_TOANCHAN1				0x0020
#define BITMASK_TOANCHAN2				0x0040
#define BITMASK_TNB						0x0050
#define BITMASK_TOONG					0x0060
#define BITMASK_SWORD					0x0080
#define BITMASK_SLASH					0x0100
#define BITMASK_BOSS					0x0200
#define BITMASK_UNDER_GROUND			0x0400
#define BITMASK_ITEM					0x0600
#define BITMASK_COIN_BAG				0x0800
#define BITMASK_COIN_BULLION			0x1000
#define BITMASK_RADA_SKILL_1			0x2000
#define BITMASK_RADA_SKILL_2			0x4000

// tag
#define TAG_HERO							50
#define TAG_FLOOR							51
#define TAG_SLASH							53
#define TAG_ITEM							54
#define TAG_DQ_TIEU_HON_CHUONG				55
#define TAG_DQ_TOAN_CHAN_KIEM_PHAP			56
#define TAG_DQ_DOC_CO_KIEM_PHAP				57
#define TAG_GAME							58
#define TAG_EAGLE							61
#define TAG_DIALOG							62
#define TAG_CL_RADA							63



// tag to save mydata
#define TAG_COIN_PARABOL					70
#define TAG_COIN_STRAIGHT					71
#define TAG_COIN_ZIGZAG						72
#define TAG_COIN_ZIGZAG2					73
#define TAG_COIN_SQUARE						74
#define TAG_COIN_CIRCLE						75
#define TAG_COIN_TIM						76

// tag b2Sprite
#define TAG_COIN							80

// tag b2Skeleton
#define TAG_COINBULLION						99
#define TAG_COINBAG							100
#define TAG_ENEMY_WOODER					101
#define TAG_ENEMY_TOANCHAN1					102
#define TAG_ENEMY_TOANCHAN2					103
#define TAG_BOSS							104
#define TAG_ENEMY_TNB						105
#define TAG_ENEMY_TOONG						106
#define TAG_ENEMY_HONGLANGBA1				107
#define TAG_ENEMY_HONGLANGBA2				108

#define KEY_ITEM_MAGNET						0
#define KEY_ITEM_DOUPLE_COIN				1

// z-order
#define ZORDER_BG							0
#define ZORDER_BG2							1
#define ZORDER_MOON							2
#define ZORDER_TMX							3
#define ZORDER_ENEMY						4
#define ZORDER_HERO							5
#define ZORDER_SMT							6


#define PTM_RATIO							32
#define MAX_DYNAMIC_HUMAN_ENEMY				8
#define MAX_BULLET_HERO_POOL				30
#define MAX_BOMB_HERO_POOL					7
#define MAX_BULLET_SOLDIER_ENEMY_POOL		3



#define	MUSIC_MENU							"sound/music/menu.mp3"
#define MUSIC_STAGE1						"sound/music/stage1.mp3"
#define MUSIC_STAGE2						"sound/music/stage2.mp3"
#define MUSIC_STAGE3						"sound/music/stage3.mp3"
#define MUSIC_STAGE4						"sound/music/stage4.mp3"
#define SOUND_DQDIE							"sound/dqdie.mp3"
#define SOUND_DQDIE							"sound/dqdie.mp3"
#define SOUND_DQHIT							"sound/dqhit.mp3"
#define SOUND_DQSKILL1							"sound/dqskill1.mp3"
#define SOUND_DQSKILL2							"sound/dqskill2.mp3"
#define SOUND_DQSKILL3							"sound/dqskill3.mp3"
#define SOUND_CLDIE								"sound/cldie.mp3"
#define SOUND_CLDIE								"sound/cldie.mp3"
#define SOUND_CLHIT								"sound/clhit.mp3"
#define SOUND_CLSKILL1							"sound/clskill1.mp3"
#define SOUND_CLSKILL2							"sound/clskill2.mp3"
#define SOUND_CLSKILL3							"sound/clskill3.mp3"

#define SOUND_TC1AT							"sound/tc1at.mp3"
#define SOUND_TC1DIE							"sound/tc1die.mp3"
#define SOUND_TC2AT							"sound/tc2at.mp3"
#define SOUND_TC2DIE							"sound/tc2die.mp3"
//#define SOUND_DQSKILL2							"sound/sound/dqskill2.mp3"


#define KEY_IS_MUSIC						"isMusic"
#define KEY_IS_SOUND						"isSound"
#define KEY_VALUE_MUSIC						"valuemusic"



#endif // __GLOBAL_H__
