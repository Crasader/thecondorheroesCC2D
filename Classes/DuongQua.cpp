#include "DuongQua.h"
#include "AudioEngine.h"
#include "Global.h"


DuongQua::DuongQua(string jsonFile, string atlasFile, float scale) : BaseHero(jsonFile, atlasFile, scale)
{
}

DuongQua * DuongQua::create(string jsonFile, string atlasFile, float scale)
{
	DuongQua* duongQua = new DuongQua(jsonFile, atlasFile, scale);
	duongQua->stateMachine = new Idling();
	
	duongQua->facingRight = true;
	return duongQua;
}

void DuongQua::run()
{
	clearTracks();
	addAnimation(0, "run", true);
	setToSetupPose();
}

void DuongQua::normalJump()
{
	clearTracks();
	addAnimation(0, "jump", true);
	setToSetupPose();
}

void DuongQua::doubleJump()
{
}

void DuongQua::landing()
{
}

void DuongQua::die()
{
}

void DuongQua::attacknormal()
{
}

void DuongQua::attackBySkill1()
{
}

void DuongQua::attackBySkill2()
{
}

void DuongQua::attackBySkill3()
{
}

void DuongQua::injured()
{
}

void DuongQua::die(Point posOfCammera)
{
}

void DuongQua::listener()
{
}

void DuongQua::update()
{
	
	if (stateMachine) {
		stateMachine->run(this);
	}
}


