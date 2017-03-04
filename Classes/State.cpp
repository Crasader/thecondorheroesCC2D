#include "State.h"
#include "BaseHero.h"


State::State()
{
}

State::~State()
{
}

void State::run(BaseHero *hero)
{
	// do nothing
}

void State::jump(BaseHero *hero)
{
	// do nothing
}

void State::land(BaseHero *hero)
{
	// do nothing
}

Running::Running()
{
}

Running::~Running()
{
}

void Running::jump(BaseHero * hero)
{
	hero->normalJump();
	hero->changeState(new Jumping());
}

void Running::land(BaseHero * hero)
{
}

Idling::Idling()
{
}

Idling::~Idling()
{
}

void Idling::run(BaseHero * hero)
{
	hero->run();
	hero->changeState(new Running());
}

Jumping::Jumping()
{
}

Jumping::~Jumping()
{
}

void Jumping::jump(BaseHero * hero)
{
}

void Jumping::land(BaseHero * hero)
{
}

DoupleJumping::DoupleJumping()
{
}

DoupleJumping::~DoupleJumping()
{
}

void DoupleJumping::land(BaseHero * hero)
{
}

Landing::Landing()
{
}

Landing::~Landing()
{
}

void Landing::jump(BaseHero * hero)
{
}

void Landing::run(BaseHero * hero)
{
}

Landing2::Landing2()
{
}

Landing2::~Landing2()
{
}

void Landing2::run(BaseHero * hero)
{
}
