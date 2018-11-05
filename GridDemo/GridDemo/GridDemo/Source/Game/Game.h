#pragma once
#include "Engine\Render.h"
#include "MapStuff\Map.h"
#include "Help_Headers\Timer.h"



class Game
{
private:
	Render m_render;
	Player m_player;
	Map m_map;
	Camera m_cam;

public:
	Game();
	~Game();

	void Update(float dt);
	void Draw();

private:
	void _handleInput(float dt);
	bool _updateCamera();
};