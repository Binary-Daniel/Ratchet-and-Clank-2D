#include "Entetys.h"
#include "Game.h"
#include "Platform/Platform.hpp"
#include "charecters.h"
#include "rectanglebutton.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265

void onInPutt(sf::RenderWindow& window, sf::Event& event, int& level);

using namespace sf;

//level värde 0 = meny
int level = 0;

bool left = false, right = false, jump = false;
float rvx = 0;
//int rvy = 0;
int jumpVal = 0;

bool fired = false;
bool bulletAlive = true;
float bulletendX, bulletendY;

//skapa ett nytt fönster i variabeln window
RenderWindow window(VideoMode(1920, 1080), "Ratchet & clank: a crack in time", Style::Fullscreen);
Menu menuframe(window.getSize().x, window.getSize().y);
Game game(window.getSize().x);

int main()
{
	util::Platform platform;

	//skapande av obeject
	//säter en icon för fönstret
	platform.setIcon(window.getSystemHandle());
	//säter fps:en till 200
	window.setFramerateLimit(200);

	//skapa en ny font och lada upp den
	Font font;
	font.loadFromFile("content/font/mini_pixel-7.ttf");
	menuframe.setFont(font);

	//spela musik
	Music music;
	if (!music.openFromFile("content/levels/menu/menu.ogg"))
		return -1; // error
	music.play();
	music.setLoop(true);

	//Menu Timing Clock
	Clock clock;
	Time passedTime;

	while (window.isOpen())
	{
		//tid
		passedTime += clock.restart();

		Event event;
		//loopa när något Event sker
		//sät inte animationer här för detta sker när du rör på musen eller tryker på en knap
		while (window.pollEvent(event))
		{
			window.display();
			onInPutt(window, event, level);
		}

		//här loopas allt efter framelimitorn så här skrivs allt som animeras eller sker utannågon input
		if (level == 1)
		{
			game.markcolision();
			game.takeDamage();
			game.SyphoidernasAI();
		}

		//förflytning av karaktären höger, vänster och upp
		if (left == true)
		{
			game.MoveRatchet(-4.f, 0.f);
			left = false;
		}
		if (right == true)
		{
			game.MoveRatchet(4, 0.f);
			right = false;
		}
		if (jumpVal > 0 && jumpVal < 50)
		{
			if (left == true)
			{
				game.MoveRatchet(-4.f, -4.f);
			}
			else if (right == true)
			{
				game.MoveRatchet(4.f, -4.f);
			}
			else
				game.MoveRatchet(0.f, -4.f);
			jumpVal++;
		}

		//skot kolision
		game.fire(fired, bulletendX, bulletendY);
		fired = game.hit(fired, bulletendX);

		game.movelevel();
		//rita upp på skärmen
		window.clear(Color::Black);
		if (passedTime <= seconds(4) && level == 0)
		{
			menuframe.drawTitle(window);
		}
		else if (level == 0)
		{
			window.clear();
			menuframe.drawMenu(window);
		}
		else if (level == 1)
		{
			window.clear(Color::White);
			game.drawLevel(window);

			game.drawBullet(window);
			game.drawRatchet(window);

			game.drawEnemys(window);
		}
		window.display();
	}
	return 0;
}

void onInPutt(sf::RenderWindow& window, sf::Event& event, int& level)
{
	switch (event.type)
	{
		case Event::Closed:
			window.close();
			break;
		case Event::MouseMoved:
			if (level == 0)
			{
				menuframe.onHover(window);
			}
			else
				//rotera karaktärens arm
				game.RotateRatchetArm();
		default:
			break;
	}
	switch (event.mouseButton.button)
	{
		case Mouse::Left:
			if (level == 0)
			{
				level = menuframe.onClik(window, level);
			}
			else if (level == 1)
			{
				//sät sanings värdet fired till sant för senare i kåden
				fired = true;
				bulletendX = game.getBulletRange().x;
				bulletendY = game.getBulletRange().y;
			}
		default:
			break;
	}
	if (event.type == Event::KeyPressed && level > 0)
	{
		//sät saningsvärdena left, right till sanna och jumpVal till 1 om respective D A W trycks ned
		if (event.key.code == Keyboard::D)
		{
			right = true;
			rvx = 4;
		}
		else
		{
			right = false;
			rvx = 0;
		}
		if (event.key.code == Keyboard::A)
		{
			left = true;
			rvx = -4;
		}
		else
		{
			left = false;
			rvx = 0;
		}
		if (event.key.code == Keyboard::W)
		{
			jumpVal = 1;
		}

		//gå tilbaka en nivå
		if (event.key.code == Keyboard::Escape)
		{
			level = 0;
		}
	}
}