#include "BaseHero.h"
#include "AudioEngine.h"
#include "Global.h"


BaseHero::BaseHero(string jsonFile, string atlasFile, float scale) : B2Skeleton(jsonFile, atlasFile, scale)
{
	
}

BaseHero * BaseHero::create(string jsonFile, string atlasFile, float scale)
{
	BaseHero* baseHero = new BaseHero(jsonFile, atlasFile, scale);
	
	return baseHero;
}




void BaseHero::run()
{

}

void BaseHero::normalJump()
{
}

void BaseHero::doubleJump()
{
}

void BaseHero::landing()
{
}

void BaseHero::die()
{
}

void BaseHero::attacknormal()
{
}

void BaseHero::attackBySkill1()
{
}

void BaseHero::attackBySkill2()
{
}

void BaseHero::attackBySkill3()
{
}

void BaseHero::injured()
{
}

void BaseHero::die(Point posOfCammera)
{
}

void BaseHero::listener()
{
}

void BaseHero::update()
{

}

void BaseHero::changeState(State * newState)
{
	delete stateMachine;
	stateMachine = newState;
}

State * BaseHero::getCurrentState()
{
	return stateMachine;
}

