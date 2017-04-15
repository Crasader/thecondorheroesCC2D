#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
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
#define BITMASK_SWORD					0x0080
#define BITMASK_SLASH					0x0100
#define BITMASK_BOSS					0x0200
#define BITMASK_UNDER_GROUND			0x0400
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
#define TAG_MAGNET							81

// tag b2Skeleton
#define TAG_COINBULLION						99
#define TAG_COINBAG							100
#define TAG_ENEMY_WOODER					101
#define TAG_ENEMY_TOANCHAN1					102
#define TAG_ENEMY_TOANCHAN2					103
#define TAG_BOSS							104

#define KEY_ITEM_MAGNET						0
#define KEY_ITEM_DOUPLE_COIN				1

// z-order
#define ZORDER_BG							0
#define ZORDER_BG2							1
#define ZORDER_ENEMY						2
#define ZORDER_HERO							3
#define ZORDER_SMT							4


#define PTM_RATIO							32
#define MAX_DYNAMIC_HUMAN_ENEMY				8
#define MAX_BULLET_HERO_POOL				30
#define MAX_BOMB_HERO_POOL					7
#define MAX_BULLET_SOLDIER_ENEMY_POOL		3

// Ref
#define KEY_SELECTED_HERO					"SelectedHero"		// integer

#define KEY_LIFE							"MyLife"
#define KEY_GOLD							"MyGold"
#define KEY_DIAMOND							"MyDiamond"


#define KEY_LOCKED_HERO_X					"LockedHero_"

// LEVEL CURRENTLY
#define KEY_LEVEL_HERO_X					"LevelHero_"		// integer (1->15)

#define KEY_HEALTH_HERO_X					"HealthHero_"		// baseHP

// BONUS
#define KEY_BONUS_SCORE_HERO_X				"BonusScoreHero_"
#define KEY_BONUS_GOLD_HERO_X				"BonusGoldHero_"


// SCORE CURRENTLY // ONCE LV UPGRADE, CALCULATE SCORE FOR CURRENT LEVEL (EX: Level 2, score 2500 -> Level 3, score 500)
#define KEY_SCORE_X							"ScoreHero_"		// follow level


// Define duration and cooldown time (if not define, get data from json)
#define KEY_DURATION_SKILL_1_HERO_X						"DurationSkill1_Hero_"
#define KEY_COOLDOWN_SKILL_1_HERO_X						"CoolDownSkill1_Hero_"

#define KEY_DURATION_SKILL_2_HERO_X						"DurationSkill2_Hero_"
#define KEY_COOLDOWN_SKILL_2_HERO_X						"CoolDownSkill2_Hero_"

#define KEY_DURATION_SKILL_3_HERO_X						"DurationSkill3_Hero_"
#define KEY_COOLDOWN_SKILL_3_HERO_X						"CoolDownSkill3_Hero_"

// from level skill we can calculate duration and cooldown
#define KEY_LEVEL_SKILL_1_HERO_X						"LevelSkill1_Hero_"
#define KEY_LEVEL_SKILL_2_HERO_X						"LevelSkill2_Hero_"
#define KEY_LEVEL_SKILL_3_HERO_X						"LevelSkill3_Hero_"


// ITEM HERE
#define NUMBER_OF_ITEM_HEALTH							"NumberOfItemHealth"
#define NUMBER_OF_ITEM_BIRD								"NumberOfItemBird"
#define NUMBER_OF_ITEM_MAGNET							"NumberOfItemMagnet"
#define NUMBER_OF_ITEM_DOUBLE_COIN						"NumberOfItemDoubleCoin"
#define NUMBER_OF_ITEM_COOL_DOWN						"NumberOfItemCoolDown"



#define	SOUND_BACKGROUND					"sound/theme_music.mp3"
#define SOUND_BULLET_NORMAL					"sound/normal bullet.mp3"
#define SOUND_BULLET_CIRCLE					"sound/F bullet.mp3"
#define SOUND_BULLET_SUPER					"sound/S Bullet.mp3"
#define SOUND_TANK_SHOOT					"sound/tank shoot .mp3"
#define SOUND_TANK_EXPLOSION				"sound/tank_explosion.mp3"
#define SOUND_PLANE_BOMB					"sound/plane_bomb.mp3"
#define SOUND_PLANE_DROP					"sound/plane_drop.mp3"

#define SOUND_TANK_MOVE						"sound/tank move.mp3"
#define SOUND_HELICOPTER					"sound/helicopter.mp3"
#define SOUND_MACHINE_GUN					"sound/machine gun.mp3"
#define SOUND_ENEMY_BULLET					"sound/enemy bullet.mp3"
#define SOUND_ENEMY_BOMB					"sound/enemy_bomb.mp3"

#define SOUND_CANON_SHOOT					"sound/cannon shoot.mp3"
#define SOUND_ENEMY_BOMB_EXPLOSION			"sound/enemy_bomb_explosion.mp3"
#define SOUND_BIG_FORT_SHOOT				"sound/boss_explosion.mp3"
#define SOUND_GET_ITEM						"sound/get item.mp3"
#define SOUND_MISSLE						"sound/missle.mp3"

#define SOUND_LOSE							"sound/sound_lose.mp3"
#define SOUND_WIN							"sound/Win.mp3"

#define SOUND_TRANSFORM						"sound/transform.mp3"
#define SOUND_TRANSFORM2					"sound/transform_2.mp3"


#endif // __GLOBAL_H__
