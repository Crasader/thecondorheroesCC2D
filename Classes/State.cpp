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

void State::attack(BaseHero * hero)
{
	// do nothing
}



// RUNNING
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
	hero->setPreviousState(new Running());
	
}

void Running::land(BaseHero * hero)
{
	hero->landing();
	hero->changeState(new Landing());
	hero->setPreviousState(new Running());
	
}

void Running::attack(BaseHero * hero)
{
	hero->attackNormal();
}





// IDLE
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
	hero->setPreviousState(new Idling());
	
}




// JUMP
Jumping::Jumping()
{
}

Jumping::~Jumping()
{
}

void Jumping::jump(BaseHero * hero)
{
	hero->doubleJump();
	hero->changeState(new DoupleJumping());
	hero->setPreviousState(new Jumping());
	
}

void Jumping::land(BaseHero * hero)
{
	hero->landing();
	hero->changeState(new Landing());
	hero->setPreviousState(new Jumping());
	
}

void Jumping::attack(BaseHero * hero)
{
	hero->attackLanding();
}


// DOUBLE JUMPP
DoupleJumping::DoupleJumping()
{
}

DoupleJumping::~DoupleJumping()
{
}

void DoupleJumping::land(BaseHero * hero)
{
	hero->landing();
	hero->changeState(new Landing2());
	hero->setPreviousState(new DoupleJumping());
	
}



// LANDING
Landing::Landing()
{
}

Landing::~Landing()
{
}

void Landing::jump(BaseHero * hero)
{
	if (hero->getNumberOfJump() > 0) {
		hero->normalJump();
		hero->changeState(new Jumping());
		
	}
	else {
		hero->doubleJump();
		hero->changeState(new DoupleJumping());
	}
	
	hero->setPreviousState(new Landing());
}

void Landing::run(BaseHero * hero)
{
	hero->run();
	hero->changeState(new Running());
	hero->setPreviousState(new Landing());
	
}

void Landing::attack(BaseHero * hero)
{
	hero->attackLanding();
}




// LANDING 2
Landing2::Landing2()
{
}

Landing2::~Landing2()
{
}

void Landing2::run(BaseHero * hero)
{
	hero->run();

	hero->changeState(new Running());
	hero->setPreviousState(new Landing2());
}

void Landing2::attack(BaseHero * hero)
{
	hero->attackLanding();
}

