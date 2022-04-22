#include "GUI/loginScene.h"
#include "App/App.h"

int main() 
{
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Courses", sf::Style::Close);
	App app;
	app.scenes.push(0);
	while (!app.scenes.empty())
	{
		int sceneNow = app.scenes.top();
		if (sceneNow == 0)
		{
			login_scene(window, app);
		}
		if (sceneNow == 1)
		{

		}
	}
}