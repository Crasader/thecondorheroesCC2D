#include "RefManager.h"
#include "Global.h"
#include "manager/JSonHeroManager.h"
#include "thirdsdkhelper\SdkboxPlay.h"


RefManager* RefManager::refManager;

RefManager::RefManager()
{
	ref = UserDefault::sharedUserDefault();

	isFirstPlay = ref->getBoolForKey(KEY_FIRST, true);
	menuTutorialUpgrade = ref->getIntegerForKey(KEY_MENU_TUT_UPGRADE, false);
	menuTutorialHero = ref->getIntegerForKey(KEY_MENU_TUT_HERO, false);
	isShowStory = ref->getBoolForKey(KEY_STORY, false);
	lastMapIdPlay = ref->getIntegerForKey(KEY_LAST_MAP_ID, 1);
	selectedHero = ref->getIntegerForKey(KEY_SELECTED_HERO, 0);
	lastPickHero = ref->getIntegerForKey(KEY_LAST_PICK_HERO, 0);
	isGetNewMap = ref->getBoolForKey(KEY_UNLOCK_MAP, false);

	currentStageUnlocked = ref->getIntegerForKey(KEY_CUR_STAGE_UNLOCKED, 4);
	currentMapUnLocked = ref->getIntegerForKey(KEY_CUR_MAP_UNLOCKED, 4);

	anchorTime = ref->getIntegerForKey(KEY_ANCHORTIME, time(0) + 180);
	lastDailyRewardTime = ref->getIntegerForKey(KEY_LAST_DAILY_REWARD_TIME, 0);
	dailyRewardCounter = ref->getIntegerForKey(KEY_DAILY_REWARD_COUNTER, 0);
	dailyRewardAvailable = ref->getBoolForKey(KEY_DAILY_REWARD_AVAILABLE, false);
	freeCoin = ref->getIntegerForKey(KEY_FREE_COIN, 0);

	numberOfLife = ref->getIntegerForKey(KEY_LIFE, 5);
	goldExplored = ref->getIntegerForKey(KEY_GOLD, 0);
	diamondBuy = ref->getIntegerForKey(KEY_DIAMOND, 0);

	numberItemHealth = ref->getIntegerForKey(NUMBER_OF_ITEM_HEALTH, 0);
	numberItemBird = ref->getIntegerForKey(NUMBER_OF_ITEM_BIRD, 1);
	numberItemMagnet = ref->getIntegerForKey(NUMBER_OF_ITEM_MAGNET, 0);
	numberItemDoubleGold = ref->getIntegerForKey(NUMBER_OF_ITEM_DOUBLE_COIN, 0);
	numberItemCoolDown = ref->getIntegerForKey(NUMBER_OF_ITEM_COOL_DOWN, 0);
	language = ref->getIntegerForKey(KEY_LANGUAGE, 0);

	// need to fix
	unLockHero(0);
	/*unLockHero(1);
	unLockHero(2);
	unLockHero(3);
	unLockHero(4);*/
	pointToCurrentHero(selectedHero);
}

RefManager::~RefManager()
{
	ref = nullptr;
	refManager = nullptr;
}

RefManager * RefManager::getInstance()
{
	if (refManager == nullptr) {
		refManager = new RefManager();
	}

	return refManager;
}

// DuongPM : create function to set selected hero
void RefManager::setSelectedHero(int index) {
	selectedHero = index;
	ref->setIntegerForKey(KEY_SELECTED_HERO, selectedHero);
	ref->flush();
}

void RefManager::setLastPickHero(int lastPickIndex)
{
	lastPickHero = lastPickIndex;
	ref->setIntegerForKey(KEY_LAST_PICK_HERO, lastPickHero);
	ref->flush();
}

void RefManager::setReachNewMap(bool value)
{
	isGetNewMap = value;
	ref->setBoolForKey(KEY_UNLOCK_MAP, isGetNewMap);
	ref->flush();
}

void RefManager::pointToCurrentHero(int index)
{
	JSHERO->readFile(language, index);
	auto m_index = StringUtils::format("%i", index);
	selectedHero = index;

	// ref->setIntegerForKey(KEY_SELECTED_HERO, selectedHero); ref->flush(); // DuongPM commented

	isLockedHero = ref->getBoolForKey((KEY_LOCKED_HERO_X + m_index).c_str(), true);
	currentLevel = ref->getIntegerForKey((KEY_LEVEL_HERO_X + m_index).c_str(), 1);
	currentScore = ref->getIntegerForKey((KEY_SCORE_X + m_index).c_str(), 0);

	currentHealth = ref->getIntegerForKey((KEY_HEALTH_HERO_X + m_index).c_str(), JSHERO->getBaseHP());

	durationSkill_1 = ref->getFloatForKey((KEY_DURATION_SKILL_1_HERO_X + m_index).c_str(), JSHERO->getDurationSkill1());
	coolDownSkill_1 = ref->getFloatForKey((KEY_COOLDOWN_SKILL_1_HERO_X + m_index).c_str(), JSHERO->getCoolDownSkill1());
	numberUseSkill_1 = ref->getIntegerForKey((KEY_NUMBER_USE_SKILL_1_HERO_X + m_index).c_str(), JSHERO->getNumberOfUseSkill1());

	durationSkill_2 = ref->getFloatForKey((KEY_DURATION_SKILL_2_HERO_X + m_index).c_str(), JSHERO->getDurationSkill2());
	coolDownSkill_2 = ref->getFloatForKey((KEY_COOLDOWN_SKILL_2_HERO_X + m_index).c_str(), JSHERO->getCoolDownSkill2());
	numberUseSkill_2 = ref->getIntegerForKey((KEY_NUMBER_USE_SKILL_2_HERO_X + m_index).c_str(), JSHERO->getNumberOfUseSkill2());

	durationSkill_3 = ref->getFloatForKey((KEY_DURATION_SKILL_3_HERO_X + m_index).c_str(), JSHERO->getDurationSkill3());
	coolDownSkill_3 = ref->getFloatForKey((KEY_COOLDOWN_SKILL_3_HERO_X + m_index).c_str(), JSHERO->getCoolDownSkill3());
	numberUseSkill_3 = ref->getIntegerForKey((KEY_NUMBER_USE_SKILL_3_HERO_X + m_index).c_str(), JSHERO->getNumberOfUseSkill3());

	bonusScore = ref->getIntegerForKey((KEY_BONUS_SCORE_HERO_X + m_index).c_str(), JSHERO->getBonusScore());
	bonusGold = ref->getIntegerForKey((KEY_BONUS_GOLD_HERO_X + m_index).c_str(), JSHERO->getBonusCoin());

	levelSkill_1 = ref->getIntegerForKey((KEY_LEVEL_SKILL_1_HERO_X + m_index).c_str(), 1);
	levelSkill_2 = ref->getIntegerForKey((KEY_LEVEL_SKILL_2_HERO_X + m_index).c_str(), 1);
	levelSkill_3 = ref->getIntegerForKey((KEY_LEVEL_SKILL_3_HERO_X + m_index).c_str(), 1);

}

void RefManager::setupMenuTutorialHero() {
	if (menuTutorialHero == false) {
		menuTutorialHero = true;
		ref->setBoolForKey(KEY_MENU_TUT_HERO, menuTutorialHero);
		ref->flush();
	}
}

void RefManager::setupMenuTutorialUpgrate() {
	menuTutorialUpgrade = true;
	ref->setBoolForKey(KEY_MENU_TUT_UPGRADE, menuTutorialUpgrade);
	ref->flush();
}

void RefManager::setDoneFirstPlay()
{
	isFirstPlay = false;
	ref->setBoolForKey(KEY_FIRST, isFirstPlay);
	ref->flush();
}

void RefManager::setShowedStory()
{
	isShowStory = true;
	ref->setBoolForKey(KEY_STORY, isShowStory);
	ref->flush();
}

void RefManager::setLastMapId(int id)
{
	lastMapIdPlay = id;
	ref->setIntegerForKey(KEY_LAST_MAP_ID, lastMapIdPlay);
	ref->flush();
}

void RefManager::unLockHero(int index)
{
	isLockedHero = false;
	ref->setBoolForKey((KEY_LOCKED_HERO_X + StringUtils::format("%i", index)).c_str(), isLockedHero);
	ref->flush();
}

void RefManager::increaseLevel()
{
	ref->setIntegerForKey((KEY_LEVEL_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), ++currentLevel);
	ref->flush();

	if ((currentLevel % 10) == 0) {
		setUpHealth(1);
	}
	increaseBonusScore(1);
	increaseBonusGold(1);
}

void RefManager::increaseStageUnlocked()
{
	ref->setIntegerForKey(KEY_CUR_STAGE_UNLOCKED, ++this->currentStageUnlocked);
	ref->flush();
}

void RefManager::setMapUnlocked(int index)
{
	this->currentMapUnLocked = index;
	ref->setIntegerForKey(KEY_CUR_MAP_UNLOCKED, this->currentMapUnLocked);
	ref->flush();
}

void RefManager::setUpHealth(int health)
{
	this->currentHealth += health;
	ref->setIntegerForKey((KEY_HEALTH_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), this->currentHealth);
	ref->flush();
}

void RefManager::setDownHealth(int health)
{
	if (this->currentHealth > health) {
		this->currentHealth -= health;
		ref->setIntegerForKey((KEY_HEALTH_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), this->currentHealth);
		ref->flush();
	}
}

void RefManager::setCurrentScoreAfterIncrease(int score)
{
	this->currentScore = score;
	ref->setIntegerForKey((KEY_SCORE_X + StringUtils::format("%i", selectedHero)).c_str(), score);
	ref->flush();
}

void RefManager::setUpScore(int score)
{
	this->currentScore += score;
	//if (SPHelper::getInstance()->isSigned())
	SPHelper::getInstance()->submitScore("score",this->getCurrentScore());
	ref->setIntegerForKey((KEY_SCORE_X + StringUtils::format("%i", selectedHero)).c_str(), this->currentScore);
	ref->flush();

}

void RefManager::increaseDurationSkill_X(int skill_What, int value)
{
	assert(value >= 1 && value <= 3);
	switch (skill_What)
	{
	case 1:
		this->durationSkill_1 += value;
		ref->setFloatForKey((KEY_DURATION_SKILL_1_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), durationSkill_1);
		ref->flush();
		break;
	case 2:
		this->durationSkill_2 += value;
		ref->setFloatForKey((KEY_DURATION_SKILL_2_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), durationSkill_2);
		ref->flush();
		break;

	case 3:
		this->durationSkill_3 += value;
		ref->setFloatForKey((KEY_DURATION_SKILL_3_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), durationSkill_3);
		ref->flush();
		break;

	default:
		assert(skill_What > 0 && skill_What <= 3);
		break;
	}
}

void RefManager::decreaseCoolDownSkill_X(int skill_What, int value)
{
	assert(value >= 1 && value <= 3);
	switch (skill_What)
	{
	case 1:
		this->coolDownSkill_1 -= value;
		ref->setFloatForKey((KEY_COOLDOWN_SKILL_1_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), coolDownSkill_1);
		ref->flush();
		break;
	case 2:
		this->coolDownSkill_2 -= value;
		ref->setFloatForKey((KEY_COOLDOWN_SKILL_2_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), coolDownSkill_2);
		ref->flush();
		break;

	case 3:
		this->coolDownSkill_3 -= value;
		ref->setFloatForKey((KEY_COOLDOWN_SKILL_3_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), coolDownSkill_3);
		ref->flush();
		break;

	default:
		assert(skill_What > 0 && skill_What <= 3);
		break;
	}
}

void RefManager::increaseNumberUseSkill_X(int skill_What, int value)
{
	assert(value >= 0 && value <= 2);
	switch (skill_What)
	{
	case 1:
		this->numberUseSkill_1 += value;
		ref->setIntegerForKey((KEY_NUMBER_USE_SKILL_1_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), numberUseSkill_1);
		ref->flush();
		break;
	case 2:
		this->numberUseSkill_2 += value;
		ref->setIntegerForKey((KEY_NUMBER_USE_SKILL_2_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), numberUseSkill_2);
		ref->flush();
		break;

	case 3:
		this->numberUseSkill_3 += value;
		ref->setIntegerForKey((KEY_NUMBER_USE_SKILL_3_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), numberUseSkill_3);
		ref->flush();
		break;

	default:
		assert(skill_What > 0 && skill_What <= 3);
		break;
	}
}

void RefManager::increaseBonusScore(int value)
{
	this->bonusScore += value;
	ref->setIntegerForKey((KEY_BONUS_SCORE_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), this->bonusScore);
	ref->flush();
}

void RefManager::increaseBonusGold(int value)
{
	this->bonusGold += value;
	ref->setIntegerForKey((KEY_BONUS_GOLD_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), this->bonusGold);
	ref->flush();
}

void RefManager::setAnchorTime(int value)
{
	anchorTime = value;
	ref->setIntegerForKey(KEY_ANCHORTIME, anchorTime);
	ref->flush();
}

void RefManager::setLanguage(int p_nLanguage)
{
	language = p_nLanguage;
	ref->setIntegerForKey(KEY_LANGUAGE, language);
	ref->flush();
}

void RefManager::updateTimeFromGoogle(int p_nTime) {
	lastDailyRewardTime = p_nTime;
	ref->setIntegerForKey(KEY_LAST_DAILY_REWARD_TIME, lastDailyRewardTime);
	ref->flush();
}

void RefManager::increaseDailyRewardCounter() {
	dailyRewardCounter++;
	ref->setIntegerForKey(KEY_DAILY_REWARD_COUNTER, dailyRewardCounter);
	ref->flush();
}

void RefManager::updateDailyRewardAvailable(bool p_bData) {
	dailyRewardAvailable = p_bData;
	ref->setBoolForKey(KEY_DAILY_REWARD_AVAILABLE, dailyRewardAvailable);
	ref->flush();
}

void RefManager::resetFreeCoin() {
	freeCoin = 1;
	ref->setIntegerForKey(KEY_FREE_COIN, freeCoin);
	ref->flush();
}

void RefManager::decreaseFreeCoin() {
	freeCoin--;
	ref->setIntegerForKey(KEY_FREE_COIN, freeCoin);
	ref->flush();
}

void RefManager::setUpLife(int life)
{
	this->numberOfLife += life;
	ref->setIntegerForKey(KEY_LIFE, this->numberOfLife);
	ref->flush();
}

void RefManager::setLife(int life)
{
	this->numberOfLife = life;
	ref->setIntegerForKey(KEY_LIFE, this->numberOfLife);
	ref->flush();
}

bool RefManager::setDownLife(int life)
{
	if (this->numberOfLife >= life) {
		this->numberOfLife -= life;
		ref->setIntegerForKey(KEY_LIFE, this->numberOfLife);
		ref->flush();
		return true;
	}

	return false;
}
	

void RefManager::setUpGoldExplored(int gold)
{
	this->goldExplored += gold;
	ref->setIntegerForKey(KEY_GOLD, this->goldExplored);
	ref->flush();
}

bool RefManager::setDownGold(int gold)
{
	if (this->goldExplored >= gold) {
		this->goldExplored -= gold;
		ref->setIntegerForKey(KEY_GOLD, this->goldExplored);
		ref->flush();
		return true;
	}

	return false;
}

void RefManager::setUpDiamondBuy(int diamond)
{
	this->diamondBuy += diamond;
	ref->setIntegerForKey(KEY_DIAMOND, this->diamondBuy);
	ref->flush();
}

bool RefManager::setDownDiamond(int diamond)
{
	if (this->diamondBuy >= diamond) {
		this->diamondBuy -= diamond;
		ref->setIntegerForKey(KEY_DIAMOND, this->diamondBuy);
		ref->flush();
		return true;
	}

	return false;
}

void RefManager::increaseLevelSkill_1()
{
	ref->setIntegerForKey((KEY_LEVEL_SKILL_1_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), ++this->levelSkill_1);
	ref->flush();
}

void RefManager::increaseLevelSkill_2()
{
	ref->setIntegerForKey((KEY_LEVEL_SKILL_2_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), ++this->levelSkill_2);
	ref->flush();
}

void RefManager::increaseLevelSkill_3()
{
	ref->setIntegerForKey((KEY_LEVEL_SKILL_3_HERO_X + StringUtils::format("%i", selectedHero)).c_str(), ++this->levelSkill_3);
	ref->flush();
}

void RefManager::increaseNumberItemHealth()
{
	ref->setIntegerForKey(NUMBER_OF_ITEM_HEALTH, ++this->numberItemHealth);
	ref->flush();
}

void RefManager::decreaseNumberItemHealth()
{
	if (this->numberItemHealth > 0) {
		ref->setIntegerForKey(NUMBER_OF_ITEM_HEALTH, --this->numberItemHealth);
		ref->flush();
	}
}

void RefManager::increaseNumberItemBird()
{
	ref->setIntegerForKey(NUMBER_OF_ITEM_BIRD, ++this->numberItemBird);
	ref->flush();
}

void RefManager::decreaseNumberItemBird()
{
	if (this->numberItemBird > 0) {
		ref->setIntegerForKey(NUMBER_OF_ITEM_BIRD, --this->numberItemBird);
		ref->flush();
	}
}

void RefManager::increaseNumberItemMagnet()
{
	ref->setIntegerForKey(NUMBER_OF_ITEM_MAGNET, ++this->numberItemMagnet);
	ref->flush();
}

void RefManager::decreaseNumberItemMagnet()
{
	if (this->numberItemMagnet > 0) {
		ref->setIntegerForKey(NUMBER_OF_ITEM_MAGNET, --this->numberItemMagnet);
		ref->flush();
	}
}

void RefManager::increaseNumberItemDoubleGold()
{
	ref->setIntegerForKey(NUMBER_OF_ITEM_DOUBLE_COIN, ++this->numberItemDoubleGold);
	ref->flush();
}

void RefManager::decreaseNumberItemDoubleGold()
{
	if (this->numberItemDoubleGold > 0) {
		ref->setIntegerForKey(NUMBER_OF_ITEM_DOUBLE_COIN, --this->numberItemDoubleGold);
		ref->flush();
	}
}

void RefManager::increaseNumberItemCoolDown()
{
	ref->setIntegerForKey(NUMBER_OF_ITEM_COOL_DOWN, ++this->numberItemCoolDown);
	ref->flush();
}

void RefManager::decreaseNumberItemCoolDown()
{
	if (this->numberItemCoolDown > 0) {
		ref->setIntegerForKey(NUMBER_OF_ITEM_COOL_DOWN, --this->numberItemCoolDown);
		ref->flush();
	}
}

void RefManager::readDataQuest(int p_nQuestIndex) {
	m_nRewardedQuestTimes = ref->getIntegerForKey((REWARDED_QUEST_X + StringUtils::format("%i", p_nQuestIndex)).c_str(), 0);
	m_nNumberQuest = ref->getIntegerForKey((NUMBER_QUEST_X + StringUtils::format("%i", p_nQuestIndex)).c_str(), 0);
}

void RefManager::updateRewardedQuestTimes(int p_nQuestIndex) {
	readDataQuest(p_nQuestIndex);
	ref->setIntegerForKey((REWARDED_QUEST_X + StringUtils::format("%i", p_nQuestIndex)).c_str(), ++this->m_nRewardedQuestTimes);
	ref->flush();
}

void RefManager::setUpNumberQuest(int p_nQuestIndex, int p_nData) {
	readDataQuest(p_nQuestIndex);
	this->m_nNumberQuest += p_nData;
	ref->setIntegerForKey((NUMBER_QUEST_X + StringUtils::format("%i", p_nQuestIndex)).c_str(), this->m_nNumberQuest);
	ref->flush();
}
