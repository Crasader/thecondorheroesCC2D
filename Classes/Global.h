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
#define BITMASK_BIRD					0x0020
#define BITMASK_SWORD					0x0080
#define BITMASK_SLASH					0x0100
#define BITMASK_BOSS					0x0200
#define BITMASK_UNDER_GROUND			0x0400
#define BITMASK_COIN_BAG				0x0800
#define BITMASK_COIN_BULLION			0x1000
#define BITMASK_RADA_SKILL_1			0x2000
#define BITMASK_RADA_SKILL_2			0x4000
#define BITMASK_DATNHIBA				0x8000


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

// tag enemy body vo hai
#define TAG_ENEMY_WOODER					101
#define TAG_ENEMY_TOANCHAN2					102
#define TAG_ENEMY_HONGLANGBA2				103
#define TAG_ENEMY_HOACDO2					104

// tag enemy body co hai

	//tag enemy tinh 
#define TAG_ENEMY_TNB						120
#define TAG_ENEMY_TOONG						121
	//tag enemy dong
#define TAG_ENEMY_TOANCHAN1					130
#define TAG_ENEMY_HONGLANGBA1				131
#define TAG_ENEMY_HOACDO1					132
#define TAG_ENEMY_DATNHIBA1					133
#define TAG_ENEMY_DATNHIBA2					134



// tag boss
#define TAG_BOSS							150

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

#define	MUSIC_MENU								"sound/music/menu.mp3"
#define MUSIC_STAGE1							"sound/music/stage1.mp3"
#define MUSIC_STAGE2							"sound/music/stage2.mp3"
#define MUSIC_STAGE3							"sound/music/stage3.mp3"
#define MUSIC_STAGE4							"sound/music/stage4.mp3"

#define SOUND_MCAT								"sound/mcattack.mp3"
#define SOUND_MCJUMP							"sound/mcjump.mp3"
#define SOUND_MCREVIVE							"sound/revive.mp3"

#define SOUND_DQDIE								"sound/dqdie.mp3"
#define SOUND_DQHIT								"sound/dqhit.mp3"
#define SOUND_DQSKILL1							"sound/dqskill1.mp3"
#define SOUND_DQSKILL2							"sound/dqskill2.mp3"
#define SOUND_DQSKILL3							"sound/dqskill3.mp3"
#define SOUND_CLDIE								"sound/cldie.mp3"
#define SOUND_CLHIT								"sound/clhit.mp3"
#define SOUND_CLSKILL1							"sound/clskill1.mp3"
#define SOUND_CLSKILL2							"sound/clskill2.mp3"
#define SOUND_CLSKILL3							"sound/clskill3.mp3"

#define SOUND_ENEMYHIT							"sound/enemyhit.mp3"
#define SOUND_TC1AT								"sound/tc1at.mp3"
#define SOUND_TC1DIE							"sound/tc1die.mp3"
#define SOUND_TC2AT								"sound/tc2at.mp3"
#define SOUND_TC2DIE							"sound/tc2die.mp3"
#define SOUND_HLB1AT							"sound/hlb1at.mp3"
#define SOUND_HLB1DIE							"sound/hlb1die.mp3"
#define SOUND_HLB2AT							"sound/hlb2at.mp3"
#define SOUND_HLB2DIE							"sound/hlb2die.mp3"
#define SOUND_HD1AT								"sound/hd1at.mp3"
#define SOUND_HD1DIE							"sound/hd1die.mp3"
#define SOUND_HD2AT								"sound/hd2at.mp3"
#define SOUND_HD2DIE							"sound/hd2die.mp3"
#define SOUND_DNBAT								"sound/dnbat.mp3"
#define SOUND_DNBDIE							"sound/dnbdie.mp3"
#define SOUND_TOONGDIE							"sound/toongdie.mp3"
#define SOUND_TNBDIE							"sound/tnbdie.mp3"


#define SOUND_BOSS1CHEM							"sound/boss1chem.mp3"
#define SOUND_BOSS1SKILL						"sound/boss1skill.mp3"
#define SOUND_BOSS1DIE							"sound/boss1die.mp3"

#define SOUND_BOSS2CHEM							"sound/boss2chem.mp3"
#define SOUND_BOSS2SKILL						"sound/boss2skill.mp3"
#define SOUND_BOSS2DIE							"sound/boss2die.mp3"

#define SOUND_BOSS3CHEM							"sound/boss3chem.mp3"
#define SOUND_BOSS3SKILL1						"sound/boss3skill1.mp3"
#define SOUND_BOSS3SKILL2						"sound/boss3skill2.mp3"
#define SOUND_BOSS3DIE							"sound/boss3die.mp3"

#define SOUND_COIN								"sound/getcoin.mp3"
#define SOUND_COINBULLION						"sound/getcoinbullion.mp3"
#define SOUND_COINBAG							"sound/getcoinbag.mp3"
#define SOUND_ITEM								"sound/getitem.mp3"
#define SOUND_BIRD								"sound/bird.mp3"

#define SOUND_FAIL								"sound/stagefail.mp3"
#define SOUND_WIN								"sound/stagewin.mp3"
#define SOUND_BTCLICK							"sound/btclick.mp3"

// DuongPM
#define SOUND_SCROLL_SLIDE						"sound/scrollslide.mp3"
#define SOUND_UNLOCK_HERO						"sound/unlockhero.mp3"
#define SOUND_UPGRADE_LEVEL						"sound/upgradelevel.mp3"

#define KEY_IS_MUSIC							"isMusic"
#define KEY_IS_SOUND							"isSound"
#define KEY_VALUE_MUSIC							"valuemusic"


#endif // __GLOBAL_H__
