#include "stdafx.h"
#include "MyGame.h"

#pragma warning(disable:4244)

CMyGame::CMyGame(void) : 
	m_player(640, 384, "boy.png", 0),
	m_background("tile_5.png")
{
}

CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	if (!IsGameMode() || m_spiders.size() == 0) return;

	Uint32 t = GetTime();

	// Update AI Agents
	bool bAttack = IsKeyDown(SDLK_LCTRL) || IsKeyDown(SDLK_RCTRL);
	if (IsKeyDown(SDLK_w) || IsKeyDown(SDLK_UP)) m_player.Input(CPlayer::UP, bAttack);
	else if (IsKeyDown(SDLK_s) || IsKeyDown(SDLK_DOWN)) m_player.Input(CPlayer::DOWN, bAttack);
	else if (IsKeyDown(SDLK_a) || IsKeyDown(SDLK_LEFT)) m_player.Input(CPlayer::LEFT, bAttack);
	else if (IsKeyDown(SDLK_d) || IsKeyDown(SDLK_RIGHT)) m_player.Input(CPlayer::RIGHT, bAttack);
	else m_player.Input(CPlayer::NO_DIR, bAttack);

	float health = 0;
	for (CSpider *pSpider : m_spiders)
	{
		health += pSpider->GetHealth();

		pSpider->SetEnemyPosition(m_player.GetPosition());

		if (Distance(pSpider->GetPosition(), m_player.GetPosition()) <= 64)
		{
			if (pSpider->GetState() == CSpider::ATTACK)
				m_player.OnAttacked();
			if (m_player.GetState() == CPlayer::ATTACK)
				pSpider->OnAttacked();
		}
	}

	if (m_player.GetHealth() <= 0 || health <= 0)
		GameOver();

	m_player.Update(t);
	for (CSpider* pSpider : m_spiders)
		pSpider->Update(t);
}

void DrawHealth(CGraphics* g, CVector pos, float w, float h, float health)
{
	if (health <= 0) return;
	g->DrawRect(CRectangle(pos.m_x - 1, pos.m_y - 1, w + 1, h + 1), CColor::Black());
	g->FillRect(CRectangle(pos.m_x, pos.m_y - 1, ceil(w * health / 100.f), h), health > 20 ? CColor::DarkGreen() : CColor::Red());
}

void CMyGame::OnDraw(CGraphics* g)
{
	// Draw background
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 3; j++)
			g->Blit(CVector(256.f*i, 256.f *j), m_background);

	m_player.Draw(g);
	DrawHealth(g, m_player.GetPosition() + CVector(-32, 32), 20, 4, m_player.GetHealth());
	for (CSpider* pSpider : m_spiders)
	{
		pSpider->Draw(g);
		DrawHealth(g, pSpider->GetPosition() + CVector(-32, 32), 20, 4, pSpider->GetHealth());
	}

	if (IsGameOver())
		if (m_player.GetHealth() <= 0)
			* g << font(48) << color(CColor::DarkRed()) << vcenter << center << "GAME OVER" << endl;
		else
			*g << font(48) << color(CColor::DarkBlue()) << vcenter << center << "YOU'RE A SPIDER KILLER!" << endl;
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	StartGame();	// exits the menu mode and starts the game mode
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
	m_spiders.delete_all();

	m_spiders.push_back(new CSpider(640, 40, "spider64.png", 0));
	m_spiders.push_back(new CSpider(80, 192, "spider64.png", 0));
	m_spiders.push_back(new CSpider(1200, 576, "spider64.png", 0));
	
	m_player.SetPosition(640, 384);
	m_player.ChangeState(CPlayer::IDLE, CPlayer::DOWN);
	m_player.SetHealth(100);
}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
}

// called when the game is over
void CMyGame::OnGameOver()
{
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();

	if (sym == SDLK_1) (*m_spiders.begin())->ChangeState(CSpider::IDLE);
	if (sym == SDLK_2) (*m_spiders.begin())->ChangeState(CSpider::PATROL);
	if (sym == SDLK_3) (*m_spiders.begin())->ChangeState(CSpider::CHASE);
	if (sym == SDLK_4) (*m_spiders.begin())->ChangeState(CSpider::ATTACK);
	if (sym == SDLK_5) (*m_spiders.begin())->ChangeState(CSpider::FLEE);
	if (sym == SDLK_6) (*m_spiders.begin())->ChangeState(CSpider::DIE);
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}

/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
