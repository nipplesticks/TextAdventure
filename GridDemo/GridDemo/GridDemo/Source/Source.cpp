#include "Engine\Render.h"
#include "MapStuff\Map.h"

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
		map.Draw();
		render.Flush(cam);
	}


	return 0;
}