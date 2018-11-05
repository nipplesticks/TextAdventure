#include "Game.h"
#include <SFML/Graphics.hpp>

Game::Game()
{
	Quad _viewport = { 0, 30, 80, 40 };
	Quad _inventorySettings = { 5, 5, 70, 30 };

	m_render.Init(_viewport);
	m_player.InitInventory(_inventorySettings);
	m_cam.setPosition(Vec{ 0,0,0 });
	m_map.Loadmap("Assets/Map.txt", &m_player);
}
Game::~Game()
{

}

void Game::Update(float dt)
{
	// Handle input for Player
	_handleInput(dt);

	// AI for characters
	// for all characters, handle AI


	// Updates characters on map
	m_map.InteractionMap(&m_player);

	// move characters
	m_player.Move();
}

void Game::Draw()
{
	m_render.Clear();

	m_map.Draw();
	m_player.Draw();

	m_render.Flush(m_cam, _updateCamera());
}

void Game::_handleInput(float dt)
{
	static bool MovedLastFrame = false;
	static bool InteractedLastFrame = false;
	static bool ToggledLastFrame = false;
	static float Timer = 0.0f;



	Timer += dt;
	bool moveThisFrame = false;
	bool interactedThisFrame = false;
	bool ToggledThisFrame = false;

	m_player.InteractRequest(interactedThisFrame);

	Vec moveDir = { 0,0,0 };

	if (GetAsyncKeyState('E'))
		interactedThisFrame = true;
	else if (GetAsyncKeyState('I'))
		ToggledThisFrame = true;

	if (interactedThisFrame && !InteractedLastFrame)
	{
		if (m_player.isInsideInventory())
			m_player.UseItem();
		else
			m_player.InteractRequest(true);
	}
	else if (ToggledThisFrame && !ToggledLastFrame)
		m_player.ToggleInventoryDraw();

	
	if (GetAsyncKeyState(VK_UP))
	{
		moveThisFrame = true;
		moveDir.y = -1;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		moveDir.y = 1;
		moveThisFrame = true;
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		moveThisFrame = true;
		moveDir.x = 1;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		moveDir.x = -1;
		moveThisFrame = true;
	}

	if ((!MovedLastFrame || Timer > 0.1f || m_player.isInsideInventory()) && moveThisFrame)
	{
		if (!m_player.isInsideInventory())
		{
			m_player.MoveRequest(moveDir);
			Timer = 0.0f;
		}
		else
		{
			if (Timer > 0.2f || !MovedLastFrame)
			{
				Timer = 0.0f;
				m_player.setSelectionDir(moveDir);
			}
		}
	}

	MovedLastFrame = moveThisFrame;
	InteractedLastFrame = interactedThisFrame;
	ToggledLastFrame = ToggledThisFrame;


}

bool Game::_updateCamera()
{
	bool redraw = false;
	Vec pos = m_player.getPosition();
	Vec camPos = m_cam.getPosition();
	const Quad & viewport = m_render.getViewport();

	if (pos.x >= viewport.width + camPos.x)
	{
		redraw = true;
		camPos.x += viewport.width;
	}
	else if (pos.x < camPos.x)
	{
		redraw = true;
		camPos.x -= viewport.width;
	}
	if (pos.y >= viewport.height + camPos.y)
	{
		redraw = true;
		camPos.y += viewport.height;
	}
	else if (pos.y < camPos.y)
	{
		redraw = true;
		camPos.y -= viewport.height;
	}
	m_cam.setPosition(camPos);
	return redraw;
}

