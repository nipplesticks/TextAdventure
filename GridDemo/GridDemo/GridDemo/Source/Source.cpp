#include "Engine\Render.h"
#include "MapStuff\Map.h"
#include "Help_Headers\Timer.h"

void HandleInput(Player * player);
bool UpdateCamera(Character * c, Camera * cam, const Quad & viewPort);
int main()
{
	Quad viewPort = { 0, 30, 80, 40 };
	Render render;
	Player player;
	Map map;
	Camera cam;
	
	render.Init(viewPort);
	cam.setPosition(Vec{ 0,0,0 });

	map.Loadmap("Assets/Map.txt", &player);

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		render.Clear();
		Vec moveDir = { 0,0,0 };
		//Update
		HandleInput(&player);



		map.InteractionMap(&player);
		player.Move();

		map.Draw();
		player.Draw();
		render.Flush(cam, UpdateCamera(&player, &cam, viewPort));
	}


	return 0;
}

void HandleInput(Player * player)
{
	static bool MovedLastFrame = false;
	static bool InteractedLastFrame = false;
	static Timer moveTimer(true);

	bool moveThisFrame = false;
	bool interactedThisFrame = false;

	player->InteractRequest(interactedThisFrame);

	Vec moveDir = { 0,0,0 };

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

	if (GetAsyncKeyState('E'))
		interactedThisFrame = true;

	if (interactedThisFrame && !InteractedLastFrame)
		player->InteractRequest(true);

	if ((!MovedLastFrame || moveTimer.Peek() > 0.1) && moveThisFrame)
	{
		moveTimer.Stop();
		player->MoveRequest(moveDir);
	}

	MovedLastFrame = moveThisFrame;
	InteractedLastFrame = interactedThisFrame;
}

bool UpdateCamera(Character * c, Camera * cam, const Quad & viewPort)
{
	bool redraw = false;
	Vec pos = c->getPosition();
	Vec camPos = cam->getPosition();

	if (pos.x >= viewPort.width + camPos.x)
	{
		redraw = true;
		camPos.x += viewPort.width;
	}
	else if (pos.x < camPos.x)
	{
		redraw = true;
		camPos.x -= viewPort.width;
	}
	if (pos.y >= viewPort.height + camPos.y)
	{
		redraw = true;
		camPos.y += viewPort.height;
	}
	else if (pos.y < camPos.y)
	{
		redraw = true;
		camPos.y -= viewPort.height;
	}
	cam->setPosition(camPos);
	return redraw;
}

