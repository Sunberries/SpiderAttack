#include "stdafx.h"
#include "Spider.h"

CSpider::CSpider(Sint16 x, Sint16 y, char* pFileBitmap, Uint32 time) : CSprite(x, y, 0, 0, time)
{
	LoadAnimation(pFileBitmap, "walk", CSprite::Sheet(4, 2).Col(0).From(0).To(1));
	LoadAnimation(pFileBitmap, "attack", CSprite::Sheet(4, 2).Col(1).From(0).To(1));
	LoadAnimation(pFileBitmap, "idle", CSprite::Sheet(4, 2).Col(2).From(0).To(1));
	LoadAnimation(pFileBitmap, "death", CSprite::Sheet(4, 2).Col(3).From(0).To(0));

	health = 100;
	state = NONE;
	ChangeState(IDLE);
}

CSpider::CSpider(Sint16 x, Sint16 y, CGraphics* pGraphics, Uint32 time) : CSprite(x, y, 0, 0, time)
{
	LoadAnimation(pGraphics, "walk", CSprite::Sheet(4, 2).Col(0).From(0).To(1));
	LoadAnimation(pGraphics, "attack", CSprite::Sheet(4, 2).Col(1).From(0).To(1));
	LoadAnimation(pGraphics, "idle", CSprite::Sheet(4, 2).Col(2).From(0).To(1));
	LoadAnimation(pGraphics, "death", CSprite::Sheet(4, 2).Col(3).From(0).To(0));
	SetAnimation("idle", 8);

	health = 100;
	state = NONE;
	ChangeState(IDLE);
}

CSpider::~CSpider(void)
{
}

void CSpider::OnAttacked()
{
	health -= 0.5f;
	if (health < 0) health = 0;
}

void CSpider::OnUpdate(Uint32 time, Uint32 deltaTime)
{
	// State-dependent actions
	switch (state)
	{
	case IDLE:
		health += 0.2f;
		break;
	case PATROL:	// take a random turn at a random frame, on average once every 60 frames
		health -= 0.05f;
		if (rand()%60 == 0)
			SetDirection((float)(rand() % 360));
		break;
	case CHASE:
		SetDirection(enemyPosition - GetPosition());
		health -= 0.1f;
		break;
	case ATTACK:
		SetDirection(enemyPosition - GetPosition());
		health -= 0.05f;
		break;
	case FLEE:
		SetDirection(enemyPosition + GetPosition());
		health -= 0.1f;
		break;
	case DIE:
		break;
	}

	// Generic behaviour: bounce from the screen margins
	if (GetX() < 32 || GetX() > 1280-32) SetVelocity(Reflect(GetVelocity(), CVector(1, 0)));
	if (GetY() < 32 || GetY() > 768-32) SetVelocity(Reflect(GetVelocity(), CVector(0, 1)));

	// if in motion, rotate so that to follow the direction of the motion; otherwise reset rotation
	if (GetSpeed() > 0)
		SetRotation(GetDirection() - 90);
	else if (state != ATTACK)
		SetRotation(0);

	// Transitions
	float enemyDistance = Distance(enemyPosition, GetPosition());
	switch (state)
	{
		
	case IDLE:
		if (health > 90) ChangeState(PATROL);
		if (enemyDistance < 200 && health > 40) ChangeState(CHASE);
		if (enemyDistance < 50) ChangeState(ATTACK);
		if (health < 1) ChangeState(DIE);
		break;
	case PATROL:
		if (enemyDistance < 200 && health > 40) ChangeState(CHASE);
		if (health < 20) ChangeState(IDLE);
		if (health < 1) ChangeState(DIE);
		break;
	case CHASE:
		if (enemyDistance > 250) ChangeState(IDLE);
		if (enemyDistance < 50) ChangeState(ATTACK);
		if (health < 30) ChangeState(FLEE);
		if (health < 1) ChangeState(DIE);
		break;
	case ATTACK:
		if (enemyDistance > 64) ChangeState(CHASE);
		if (health < 30) ChangeState(FLEE);
		if (health < 1) ChangeState(DIE);
		break;
	case FLEE:
		if (health < 10 || enemyDistance > 250) ChangeState(IDLE);
		if (health < 1) ChangeState(DIE);
		break;
	case DIE:
		break;
	}

	CSprite::OnUpdate(time, deltaTime);
}

void CSpider::ChangeState(STATE newState)
{
	if (newState == state)
		return;		// No state change

	state = newState;

	switch (state)
	{
	case IDLE:
		SetVelocity(0, 0);	
		SetAnimation("idle", 4);
		break;
	case PATROL:
		SetDirection((float)(rand() % 360));
		SetSpeed(PATROL_SPEED);
		SetAnimation("walk");
		break;
	case CHASE:
		SetSpeed(CHASE_SPEED);
		SetAnimation("walk");
		break;
	case ATTACK:
		SetSpeed(PATROL_SPEED);
		SetAnimation("attack");
		break;
	case FLEE:
		SetSpeed(CHASE_SPEED);
		SetAnimation("walk");
		break;
	case DIE:
		SetVelocity(0, 0);
		SetAnimation("death");
		break;
	}
}
