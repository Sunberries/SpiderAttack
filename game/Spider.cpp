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
		break;
	case PATROL:	// take a random turn at a random frame, on average once every 60 frames
		break;
	case CHASE:
		break;
	case ATTACK:
		break;
	case FLEE:
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
		break;
	case PATROL:
		break;
	case CHASE:
		break;
	case ATTACK:
		break;
	case FLEE:
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
		SetAnimation("walk");
		break;
	case CHASE:
		SetAnimation("walk");
		break;
	case ATTACK:
		SetAnimation("attack");
		break;
	case FLEE:
		SetAnimation("walk");
		break;
	case DIE:
		SetAnimation("death");
		break;
	}
}
