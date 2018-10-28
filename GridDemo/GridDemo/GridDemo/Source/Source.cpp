#include "Engine\Render.h"
#include "MapStuff\Map.h"
#include "Help_Headers\Timer.h"

int main()
{
	Quad viewPort = { 0, 0, 20, 10 };
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
		if (GetAsyncKeyState(VK_UP))
		{
			moveDir.y = 1;
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			
		}

		if (GetAsyncKeyState(VK_RIGHT))
		{
			
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			
		}


		map.Draw();
		player.Draw();
		render.Flush(cam);
	}


	return 0;
}