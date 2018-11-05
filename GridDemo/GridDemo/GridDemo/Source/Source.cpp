#include "LevelEditor/Source/GUI/Button.h"
#include "LevelEditor/LevelEditor.h"
#include "Game/Game.h"


void GameLoop();
void LevelEditorLoop();
enum Button_Types
{
	StartGame = 0,
	StartLevelEditor,
	Exit
};

enum Menu_Choice
{
	MainMenu = 0,
	RunGame,
	RunLevelEditor
};

int main()
{

	sf::Vector2i windowSize = { 640, 480 };
	sf::RenderWindow * mainMenu = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), "Text adventure");
	Button buttons[3];

	buttons[StartGame].setText("Start Game");
	buttons[StartLevelEditor].setText("Level Editor");
	buttons[Exit].setText("Exit");


	sf::Vector2i buttonSize = {300, 100};

	for (int i = 0; i < 3; i++)
	{
		buttons[i].setSize((float)buttonSize.x, (float)buttonSize.y);
		buttons[i].setViewPosition(windowSize.x / 2 - buttonSize.x / 2, 25 + (((windowSize.y - 50) / 2) - buttonSize.y / 2) * i);
	}

	bool stillPlaying = true;
	Menu_Choice mc = MainMenu;
	while (stillPlaying)
	{
		static bool Mouse_Left_Pressed_last_frame = false;
		
		if (mc == RunLevelEditor)
		{
			mainMenu->close();
			delete mainMenu;
			LevelEditorLoop();
			mainMenu = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), "Text adventure");
			mc = MainMenu;
		}
		else if (mc == RunGame)
		{
			mainMenu->close();
			delete mainMenu;
			GameLoop();
			mainMenu = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), "Text adventure");
			mc = MainMenu;
			system("CLS");
		}


		sf::Event e;
		while (mainMenu->pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				mainMenu->close();
		}
		
		bool mouseLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

		sf::Vector2i mp = sf::Mouse::getPosition(*mainMenu);
		for (int i = 0; i < 3; i++)
		{
			buttons[i].setState(Button::Button_State::Standard);

			if (buttons[i].PointIsInside((float)mp.x, (float)mp.y))
			{
				if (mouseLeftPressed)
				{
					buttons[i].setState(Button::Button_State::Press);
				}
				else
				{
					if (Mouse_Left_Pressed_last_frame)
					{
						switch (i)
						{
						case StartGame:
							mc = RunGame;
							break;
						case StartLevelEditor:
							mc = RunLevelEditor;
							break;
						case Exit:
							stillPlaying = false;
							break;
						}
					}
					else
					{
						buttons[i].setState(Button::Button_State::Hover);	
					}
				}



			}
		}

		Mouse_Left_Pressed_last_frame = mouseLeftPressed;



		mainMenu->clear();
		for (int i = 0; i < 3; i++)
			mainMenu->draw(buttons[i]);
		mainMenu->display();
	}
	delete mainMenu;
	return 0;
}

void GameLoop()
{
	Game g;
	Timer dt(true);
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		g.Update(dt.Stop());
		g.Draw();
	}
}

void LevelEditorLoop()
{
	LevelEditor le;
	Timer dt(true);
	while (le.WinodwIsOpen())
	{
		le.Update(dt.Stop());
		le.Draw();
	}
}
